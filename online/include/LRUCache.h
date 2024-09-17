#ifndef __LRUCACHE_H__
#define __LRUCACHE_H__

#include <unordered_map>
#include <string>
#include <list>
#include <mutex>

using std::string;
using std::unordered_map;
using std::iterator;
using std::list;
using std::mutex;

class LRUCache{

public:
    LRUCache(int num = 100);
    LRUCache(const LRUCache & cache);

    void addElement(const string & key,const string & value);
    void readFromFile(const string & filename);
    list<string,string> & getPendingUpdateList();

private:
    unordered_map<string,list<string,string>::iterator> _hashMap;
    list<string,string> _resultsList;
    list<string,string> _pendingUpdateList;
    int _capacity;
    mutex _mutex;
};

#endif // LRUCACHE_H__