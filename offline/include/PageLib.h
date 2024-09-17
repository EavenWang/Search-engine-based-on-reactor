#ifndef __PAGELIB_H__
#define __PAGELIB_H__

#include "../../library/simhash/Simhasher.hpp"
#include "Configuration.h"

#include "DirScanner.h"
#include <map>
#include <unordered_map>
#include <set>

using std::map;
using std::pair;
using std::unordered_map;
using std::set;
using namespace simhash;

struct RssItem{
    int id;
    string title;
    string link;
    string description;
    string content;
};

class PageLib{

public:
    PageLib(Configuration * conf,DirScanner & dirScanner);
    
    void create();

private:
    void store(const RssItem & rss);
    bool cutRedundantPage(RssItem & rss);
    bool process(string fileName);

    void readChineseStopWords(unordered_set<string> & stopWordsSet);
    void readEnglishStopWords(unordered_set<string> & stopWordsSet);

private:
    DirScanner & _dirScanner;

    Configuration * _conf;

    vector<string> _pages;

    map<int,pair<int,int>> _offsetLib;
 
    vector<uint64_t> _fingerprint; //存储网页库中已经存在的网页的hash值

    unordered_set<string>  _stopWords;
    Simhasher _simhasher;
};


#endif // PAGELIB_H__