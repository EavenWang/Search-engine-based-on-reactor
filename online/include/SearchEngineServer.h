#ifndef __SEARCHENGINESERVER_H__
#define __SEARCHENGINESERVER_H__

#include <workflow/WFFacilities.h>
#include <wfrest/HttpServer.h>
#include "KeyRecommander.h"
#include "WebPageQuery.h"


class SearchEngineServer{
public:

    SearchEngineServer(Dictionary * dict,WebPageQuery * query)
    :_dictionary(dict)
    ,_query(query)
    {
    }
    
    void start();
    
private:
    void listen();
private:
    wfrest::HttpServer _server;
    Dictionary * _dictionary;
    WebPageQuery *_query;
};

#endif // SEARCHENGINESERVER_H__