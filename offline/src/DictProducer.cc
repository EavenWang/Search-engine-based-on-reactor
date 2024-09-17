#include "../include/DictProducer.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <unordered_set>

using std::cout;
using std::cerr;
using std::ifstream;
using std::istringstream;
using std::ofstream;
using std::unordered_set;

#define INITSIZE 1024

std::unordered_set<string> chinese_punctuation = {
        "。", "，", "；", "：", "！", "？", "（", "）",
        "《", "》", "“", "”","、", "……", "——","【","】",
        "·","-","‘","’","·"
        };

size_t nBytesCode(const char ch) {
    if (ch & (1 << 7)) {
        int nBytes = 1;
        for (int idx = 0; idx != 6; ++idx) {
            if (ch & (1 << (6 - idx))) {
                ++nBytes;
            } else
                break;
        }
        return nBytes;
    }
    return 1;
}

DictProducer::DictProducer(Configuration & conf,vector<string> & files, SplitTool *tool)
:_conf(conf)
, _cuttor(tool)
{
    _files.reserve(INITSIZE);
    _dict.reserve(INITSIZE);
    _files = files;
}


void dealEnglishWord(string &word)
{
    for (size_t idx = 0; idx != word.size(); idx++)
    {
        if (!isalpha(word[idx]))
        {
            word[idx] = ' ';
        }else{
            word[idx] = tolower(word[idx]);
        }
    }
}

void DictProducer::readEnglishStopWords(unordered_set<string> & stopWordsSet){
    ifstream ifs(_conf.json()["stopWordsOfEnglish"]);

    if(!ifs){
        cerr << "error in readEnglishStopWords";
        return;
    }

    string word;
    string line;
    while (getline(ifs, line))
    {
        dealEnglishWord(line);
        istringstream iss(line);
        string word;
        while (iss >> word)
        {
            if (string() != word)
            {
                stopWordsSet.insert(word);
            }
        }
    }
    ifs.close();
}

void readEnglishFile(const string &filePath, map<string, int> &wordMap)
{
    ifstream ifs(filePath);
    if (!ifs)
    {
        cerr << "ifs open " << filePath << " error!" << "\n";
        return;
    }

    string line;

    while (getline(ifs, line))
    {
        dealEnglishWord(line);
        istringstream iss(line);
        string word;
        while (iss >> word)
        {
            if (string() != word)
            {
                ++wordMap[word];
            }
        }
    }
    ifs.close();
}

void DictProducer::buildEnDict()
{
    map<string, int> wordMap;
    for (int i = 0; i < _files.size(); i++)
    {
        string filePath = _files[i];
        readEnglishFile(filePath, wordMap);
    }

    unordered_set<string> stopWords;
    readEnglishStopWords(stopWords);

    for (const auto & wordPair : wordMap)
    {
        if(stopWords.find(wordPair.first) != stopWords.end()){
            continue;
        }
        _dict.push_back(wordPair);
    }
}

string dealChinesehWord(string &word)
{
    string newWord;
    for (size_t idx = 0; idx != word.size(); idx++)
    {
        if (word[idx] != '\r' && word[idx] != ' ' && word[idx] != '\n' && word[idx] != '\t')
        {
            newWord += word[idx];
        }
    }
    return newWord;
}

void DictProducer::readChineseStopWords(unordered_set<string> & stopWordsSet){

    ifstream ifs(_conf.json()["stopWordsOfChinese"]);

    if(!ifs){
        cerr << "error in readChineseStopWords";
        return;
    }

    string word;
    string newWord;
    while(getline(ifs,word)){
        newWord = dealChinesehWord(word);
        stopWordsSet.insert(newWord);
    }
    ifs.close();
}

void DictProducer::buildCnDict()
{
    map<string, int> wordMap;

    for (int i = 0; i < _files.size(); i++)
    {
        string filePath = _files[i];
        ifstream ifs(filePath);
        if (!ifs)
        {
            cerr << "ifs open " << filePath << " error!" << "\n";
            return;
        }

        string line;
        string sentence;
        while (getline(ifs, line))  sentence += line;

        sentence = dealChinesehWord(sentence);
        vector<string> words = _cuttor->cut(sentence);

        for(int i = 0;i < words.size();i++){
            if(nBytesCode(words[i][0]) == 1 || 
            chinese_punctuation.find(words[i]) != chinese_punctuation.end()) continue;
            wordMap[words[i]]++;
        }
        ifs.close();
    }

    unordered_set<string> stopWords;
    readChineseStopWords(stopWords);

    for (const auto & wordPair : wordMap)
    {   
        if(stopWords.find(wordPair.first) != stopWords.end()){
            continue;
        }
        _dict.push_back(wordPair);
    }
}

void DictProducer::createEnglishIndex()
{   
    string words;

    for(int i = 0;i < _dict.size();i++){
        words = _dict[i].first;

        for(int j = 0;j < words.size();j++){
            _index[string(1,words[j])].insert(i);
        }
    }
}

void DictProducer::createChineseIndex(){
    string words;
    vector<string> chars;
    for(int i = 0;i < _dict.size();i++){
        words = _dict[i].first;
        chars = _cuttor->cutToChar(words);
        for(int j = 0;j < chars.size();j++){
            _index[chars[j]].insert(i);
        }
    }
}


void DictProducer::storeEnglishDict()
{
    ofstream ofs(_conf.json()["EnglishDict"]);
    if (!ofs)
    {
        cerr << "error in storeEnglishDict\n";
        return;
    }

    for (int i = 0; i < _dict.size(); i++)
    {   
        ofs << _dict[i].first << " " << _dict[i].second << "\n";
    }
    ofs.close();

    ofs.open(_conf.json()["dictIndex"],std::ios::app);
    if (!ofs)
    {
        cerr << "error in storeEnglishDict\n";
        return;
    }

    for(const auto & mapPair : _index){

        ofs << mapPair.first << " ";
        for(const auto & ele : mapPair.second){
            ofs << ele << " ";
        }
        ofs << "\n";
    }
    ofs.close();
}

void DictProducer::storeChineseDict()
{
    ofstream ofs(_conf.json()["ChineseDict"]);
    if (!ofs)
    {
        cerr << "error in storeChineselishDict\n";
        return;
    }

    for (int i = 0; i < _dict.size(); i++)
    {   
        ofs << _dict[i].first << " " << _dict[i].second << "\n";
    }
    ofs.close();

    ofs.open(_conf.json()["dictIndex"],std::ios::app);
    if (!ofs)
    {
        cerr << "error in storeChineseDict\n";
        return;
    }

    for(const auto & mapPair : _index){

        ofs << mapPair.first << " ";
        for(const auto & ele : mapPair.second){
            ofs << ele << " ";
        }
        ofs << "\n";
    }
    ofs.close();
}