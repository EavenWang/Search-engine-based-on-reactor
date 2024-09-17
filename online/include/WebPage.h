#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__

#include <string>
#include <map>

using std::string;
using std::map;

const static int TOPK_NUMBER = 20;

class WebPage{
public:
    
    int getDocId();
    string getDoc();
    map<string,int> & getWordsMap();

private:
    string _doc;
    int _docId;
    string _docTitle;
    string _docUrl;
    string _docContent;
};


#endif // WEBPAGE_H__