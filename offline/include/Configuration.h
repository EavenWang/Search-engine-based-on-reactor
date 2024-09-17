#ifndef _CONFIGURATION_H__
#define _CONFIGURATION_H__

#include "../../library/nlohmann/json.hpp"

#include <string>
#include <fstream>
#include <map>
#include <stdio.h>
#include <string.h>
#include <iostream>

using std::string;
using std::ifstream;
using std::map;
using std::cerr;
using std::cout;

class Configuration
{
public:
    static Configuration * getInstance(){
        if(_conf == nullptr){
            _conf = new Configuration("/home/mylinux/homework/SearchEngineOnReactor/conf/conf.json");
        }
        return _conf;
    }

    static void destroy(){
        if(_conf){
            delete _conf;
            _conf = nullptr;
        }
    }
    
    nlohmann::json & json(){
        return _js;
    }

private:
    Configuration(const string & configPath)
    {
        ifstream ifs(configPath);
        if(!ifs){
            cerr << "error in open conf";
        }
        ifs >> _js;
        ifs.close();
    }

private:
    nlohmann::json _js;
    static Configuration * _conf;
};

#endif // _CONFIGURATION_H__