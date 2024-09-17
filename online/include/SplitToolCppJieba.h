#ifndef __SPLITTOOLCPPJIEBA_H__
#define __SPLITTOOLCPPJIEBA_H__

#include "SplitTool.h"

#include "../../library/cppjieba/Jieba.hpp"
#include "Configuration.h"
#include <string>

using std::string;

class SplitToolJieba
:public SplitTool 
{
public: 
    static SplitToolJieba * getIntance(Configuration & conf){
        if(_tool == nullptr){
            _tool = new SplitToolJieba(conf);
            return _tool;
        }

        return _tool;
    }

    static void destroy(){
        if(_tool != nullptr){
            delete _tool;
            _tool = nullptr;
        }
    }

    vector<string> cut(const string sentence) override{
        vector<string> words;
        _jieba->Cut(sentence,words);
        return words;                    
    }

    vector<string> cutToChar(const string sentence) override{
        vector<string> chars;
        _jieba->CutSmall(sentence,chars,1);
        return chars;
    }

private:
    SplitToolJieba(Configuration & conf){
        string DICT_PATH = conf.json()["jieba"]["dict"];
        string HMM_PATH = conf.json()["jieba"]["hmm"];
        string USER_DICT_PATH = conf.json()["jieba"]["user"];
        string IDF_PATH = conf.json()["jieba"]["idf"];
        string STOP_WORD_PATH = conf.json()["jieba"]["stop"];
        _jieba = new cppjieba::Jieba(DICT_PATH,HMM_PATH,
                                        USER_DICT_PATH,IDF_PATH,
                                        STOP_WORD_PATH);
    }

private:
    cppjieba::Jieba  * _jieba;
    static SplitToolJieba * _tool;
};
SplitToolJieba * SplitToolJieba::_tool = nullptr;

#endif // SPLITTOOLCPPJIEBA_H__