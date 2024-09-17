#ifndef __WEBPAGEQUERY_H__
#define __WEBPAGEQUERY_H__

#include "SplitTool.h"
#include "Configuration.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <queue>


using std::vector;
using std::unordered_map;
using std::pair;
using std::string;
using std::set;
using std::unordered_set;
using std::priority_queue;
using std::shared_ptr;

class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;

struct WebPage{
    string _doc; 
    double cosinSimilarity;
};

struct WebPageCompare {
    bool operator()(const WebPage& lhs, const WebPage& rhs) const {
        // 大根堆，cosinSimilarity 较大的在前面
        return lhs.cosinSimilarity < rhs.cosinSimilarity;
    }
};

class WebPageQuery{
public:
    WebPageQuery(SplitTool * tool,Configuration * conf);

    vector<string> doQuery(const string & str);

    vector<pair<int,double>> getCosinSimilarity(const vector<string> & queryWords);

    void executeQuery(vector<pair<int,double>> & docSimilarity,Configuration * conf);

    void response(TcpConnectionPtr & tcp);

private:
    vector<pair<int,int>> _offsetLib;//网页偏移库
    unordered_map<string,set<pair<int,double>>> _invertIndex; //倒排索引
    unordered_set<string> _stopCn;
    unordered_set<string> _stopEn;
    priority_queue<WebPage,vector<WebPage>, WebPageCompare> _priWebs;
    SplitTool * _tool;
};

#endif // WEBPAGEQUERY_H__