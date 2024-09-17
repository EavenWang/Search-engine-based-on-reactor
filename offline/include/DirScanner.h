#ifndef __DIRSCANNER_H__
#define __DIRSCANNER_H__

#include <vector>
#include <string>

using std::vector;
using std::string;

#define INITSIZE 1024

class DirScanner{
public:
    DirScanner(){
        _files.reserve(INITSIZE);
    }

    //返回文件路径数组
    vector<string> & getFiles();
    
    //遍历目录，得到文件路径数组
    void traverse(string dir);
private:
    vector<string> _files;
};


#endif // DIRSCANNER_H__