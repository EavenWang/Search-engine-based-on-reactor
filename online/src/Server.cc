#include "../include/Server.h"
#include "../include/EventLoop.h"
#include "../include/TcpConnection.h"
#include "../include/SplitToolCppJieba.h"

#include <iostream>
#include <functional>

using std::cout;
using std::endl;
using std::bind;

MyTask::MyTask(const string &msg, const TcpConnectionPtr &con)
: _msg(msg)
, _con(con)
{

}

void MyTask::process(Dictionary * dictionary,WebPageQuery * query,Configuration * conf)
{
    //处理业务逻辑
    _msg.pop_back();
    nlohmann::json js = nlohmann::json::parse(_msg);

    if(js.contains("suggestions")){

        _msg = js["suggestions"];
        KeyRecommander krc(_msg,dictionary);
        krc.execute();
        krc.response(_con);
    }else if(js.contains("search")){
        _msg = js["search"];
        vector<string> words = query->doQuery(_msg);
        vector<pair<int,double>> articleIdAndCosin = query->getCosinSimilarity(words);
        query->executeQuery(articleIdAndCosin,conf);
        query->response(_con);
    }
}

myServer::myServer(size_t threadNum, size_t queSize
                       , const string &ip
                       , unsigned short port)
: _pool(threadNum, queSize)
, _server(ip, port)
{
}

myServer::~myServer()
{

}

//服务器的启动与停止
void myServer::start()
{   
    _conf = Configuration::getInstance();
    _tool = SplitToolJieba::getIntance(*_conf);
    _dictionary = Dictionary::getInstance();
    _dictionary->init(_conf);
    _query = new WebPageQuery(_tool,_conf);
    _pool.start();

    //注册所有的回调
    using namespace std::placeholders;
    _server.setAllCallback(std::bind(&myServer::onNewConnection, this, _1)
                           , std::bind(&myServer::onMessage, this, _1)
                           , std::bind(&myServer::onClose, this, _1));

    _server.start();
}

void myServer::stop()
{
    _pool.stop();
    _server.stop();
}

void myServer::onNewConnection(const TcpConnectionPtr &con)
{   
    
    cout << con->toString() << " has connected!!!" << endl;

}
void myServer::onMessage(const TcpConnectionPtr &con)
{
    //接收客户端的数据
    string msg = con->receive();
    cout << ">>recv msg from client: " << msg;
    MyTask task(msg, con);

    _pool.addTask(std::bind(&MyTask::process, task,_dictionary,_query,_conf));

}

void myServer::onClose(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has closed!!!" << endl;
}
