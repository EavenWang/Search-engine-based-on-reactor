#ifndef __INVERTINDEX_H__
#define __INVERTINDEX_H__

#include <map>
#include <string>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include "../include/SplitTool.h"
#include "../include/Configuration.h"

using std::map;
using std::string;
using std::unordered_map;
using std::pair;
using std::set;
using std::unordered_set;


class InvertIndex{

public:

    InvertIndex(SplitTool * tool,Configuration * conf)
    :_numOfArticles(0)
    ,_tool(tool)
    ,_conf(conf)
    {
        readChineseStopWords(_stopWords);
    }

    void processDoc(string filename);   //读取xml文件，并建立倒排索引

    void computeWeight();

    void store(string filename);   //存储倒排索引

private:
    void computeSingleWeight(int id,string & sentence);
    void readChineseStopWords(unordered_set<string> & stopWordsSet);

private:

    map<string,set<pair<int,int>>> _wordsFrequency;    //词语对应的文章docid，在文章中的频次

    map<int,double> _articleWeights; //文章docid 对应的所有权重的平方和

    unordered_map<string,set<pair<int,double>>> _invertIndexTable;  //词语对应的文章id和权重
    int _numOfArticles;
    SplitTool * _tool;

    Configuration  * _conf;

    unordered_set<string> _stopWords;
};


#endif // INVERTINDEX_H__