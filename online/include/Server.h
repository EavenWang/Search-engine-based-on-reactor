#ifndef __ECHOSERVER_H__
#define __ECHOSERVER_H__

#include "ThreadPool.h"
#include "TcpServer.h"
#include "WebPageQuery.h"
#include "KeyRecommander.h"

class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con);
    void process(Dictionary * dictionary,WebPageQuery * query,Configuration * conf);

private:
    string _msg;
    TcpConnectionPtr _con;
};

class myServer
{
public:
    myServer(size_t threadNum, size_t queSize
               , const string &ip
               , unsigned short port);
    ~myServer();

    //服务器的启动与停止
    void start();
    void stop();

    void onNewConnection(const TcpConnectionPtr &con);
    void onMessage(const TcpConnectionPtr &con);
    void onClose(const TcpConnectionPtr &con);

private:
    ThreadPool _pool;
    TcpServer _server;
    Configuration * _conf;
    SplitTool * _tool;
    Dictionary * _dictionary;
    WebPageQuery *_query;
};

#endif
