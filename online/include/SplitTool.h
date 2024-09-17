#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__

#include <vector>
#include <string>

using std::vector;
using std::string;

class SplitTool
{
public:
    virtual vector<string> cut(const string sentence) = 0;
    virtual vector<string> cutToChar(const string sentence) = 0;
};

#endif // SPLITTOOL_H__