#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include "SplitTool.h"
#include "Configuration.h"

#include <vector>
#include <map>
#include <string>
#include <set>
#include <unordered_set>

using std::vector;
using std::string;
using std::map;
using std::pair;
using std::set;
using std::unordered_set;


class DictProducer
{
public:
    DictProducer(Configuration & conf,vector<string>& files,SplitTool * tool);

    void buildEnDict();
    void buildCnDict();

    void createEnglishIndex();
    void createChineseIndex();

    void storeEnglishDict();
    void storeChineseDict();

private:
    void readChineseStopWords(unordered_set<string> & stopWordsSet);
    void readEnglishStopWords(unordered_set<string> & stopWordsSet);

private:
    Configuration & _conf;
    vector<string> _files;  //文件的路径的数组
    vector<pair<string,int>> _dict;     //词典
    map<string,set<int>> _index;    //词典索引
    SplitTool * _cuttor;    //分词工具
};


#endif // DICTPRODUCER_H__