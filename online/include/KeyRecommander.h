#ifndef __KEYRECOMMANDER_H__
#define __KEYRECOMMANDER_H__

#include "Configuration.h"

#include <string>
#include <queue>
#include <vector>
#include <set>
#include <map>

using std::shared_ptr;

class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;

using std::string;
using std::priority_queue;
using std::pair;
using std::map;
using std::vector;
using std::set;

struct CandidateResult{
    string _word;
    int _freq;
    int _dist;
};

struct CompareCandidate {
    bool operator()(const CandidateResult& lhs, const CandidateResult& rhs) {
        if (lhs._dist != rhs._dist)
            return lhs._dist > rhs._dist;
        return lhs._freq < rhs._freq;
    }
};

class Dictionary{

public:
    static Dictionary * getInstance();
    void init(Configuration * conf);
    map<string,set<int>> & getIndexTable();
    vector<pair<string,int>> & getEnglishDict();
    vector<pair<string,int>> & getChineseDict();
    void doQuery(const string & word,set<string> & result);

public:
    Dictionary();

private:
    vector<pair<string,int>> _EnglishDict;
    vector<pair<string,int>> _ChineseDict;
    map<string,set<int>> _index;
    static Dictionary * dictionary;
};

class KeyRecommander{
public:
    KeyRecommander(string & word,Dictionary * dictionary)
    :_queryWord(word)
    ,_dictionary(dictionary)
    {}

    void execute();
    void response(TcpConnectionPtr & tcp);

private:
    vector<pair<string,int>> cutWords();
    int distance(const string &rhs);

private:
    string _queryWord;
    Dictionary * _dictionary;
    priority_queue<CandidateResult,std::vector<CandidateResult>,CompareCandidate> _prique;
};


#endif // KEYRECOMMANDER_H__ 