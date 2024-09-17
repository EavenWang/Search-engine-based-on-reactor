#include "./../include/SearchEngineServer.h"

#include <wfrest/json.hpp>
#include <iostream>
#include <signal.h>
#include <wfrest/json.hpp>
#include <string>

using std::cout;
using std::string;
using std::to_string;
using std::cerr;

static WFFacilities::WaitGroup waitGroup(1);

void sighandler(int signum){
    cout << "over!\n";
    waitGroup.done();
}

void SearchEngineServer::listen(){
    _server.GET("/",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->File("/home/mylinux/homework/project/online/html/client.html");
    });

    _server.POST("/suggestions",[=](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        
        const void * body;
        size_t size;

        req->get_parsed_body(&body,&size);

        string js = static_cast<const char * >(body);

        nlohmann::json json_object = nlohmann::json::parse(js);

        string query = json_object["query"];
        //cout << query << "\n";
        
        KeyRecommander rec(query,_dictionary);

        rec.execute();
        rec.response(resp);
        //resp->append_output_body(R"({"suggestions":["Suggestion 1", "Suggestion 2", "Suggestion 3"]})");
        resp->headers["Content-Type"] = "application/json";
    });

    _server.POST("/search",[=](const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork * series){
        
        const void * body;
        size_t size;

        req->get_parsed_body(&body,&size);

        string js = static_cast<const char * >(body);

        nlohmann::json json_object = nlohmann::json::parse(js);

        string search = json_object["search"];
        cout << search << "\n";
        vector<string> words = _query->doQuery(search);
        
        WFRedisTask * redisTask = WFTaskFactory::create_redis_task(
            "redis://127.0.0.1:6379",10,
            [=](WFRedisTask * redisTask){

                int state = redisTask->get_state(); //获取状态
                int error = redisTask->get_error(); //获取errno
                switch (state){
                case WFT_STATE_SYS_ERROR: //操作系统层面的错误
                    cerr << "system error: " << strerror(error) << "\n";
                    break;
                case WFT_STATE_DNS_ERROR: //网络信息错误
                    cerr << "DNS error: " << gai_strerror(error) << "\n";
                    break;
                case WFT_STATE_SUCCESS:
                    break;
                }

                protocol::RedisResponse * respRedis = redisTask->get_resp();
                protocol::RedisValue result;
                respRedis->get_result(result);

                if(result.is_nil()){
                    cout << "visit disk\n";
                    vector<pair<int,double>> articleIdAndCosin = _query->getCosinSimilarity(words);
                    _query->executeQuery(articleIdAndCosin);
                    _query->response(resp,series,search);
                }else{
                    cout << "visit redis\n";
                    resp->append_output_body(result.string_value());
                }
            }
        );

        redisTask->get_req()->set_request("GET",{search});
        series->push_back(redisTask);

        resp->headers["Content-Type"] = "application/json";
    });

}

void SearchEngineServer::start(){
    signal(SIGINT,sighandler);

    listen();
    if(_server.track().start(12345) == 0){
        // start是非阻塞的
        _server.list_routes();
        waitGroup.wait();
        cout << "finished!\n";
        _server.stop();
    }else{
        perror("server start fail!");
    }
}