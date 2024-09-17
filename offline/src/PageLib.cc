#include "../include/PageLib.h"
#include "../../library/tinyxml/tinyxml2.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <math.h>

using namespace tinyxml2;
using std::ofstream;
using std::cerr;
using std::regex;
using std::cout;


regex tag_regex("<[^>]*>");
static int id = 0;

string dealChinesehWord(string &word)
{
    string newWord;
    for (size_t idx = 0; idx != word.size(); idx++)
    {   
        if (word[idx] != '\r' && word[idx] != '\n' && word[idx] != ' ' && word[idx] != '\t')
        {
            newWord += word[idx];
        }
    }
    return newWord;
}

void PageLib::readChineseStopWords(unordered_set<string> & stopWordsSet){

    ifstream ifs(_conf->json()["stopWordsOfChinese"]);

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

PageLib::PageLib(Configuration * conf,DirScanner & dirScanner)
    :_dirScanner(dirScanner)
    ,_conf(conf)
    ,_simhasher(conf->json()["simhash"]["dict"],
            conf->json()["simhash"]["hmm"],
            conf->json()["simhash"]["idf"],
            conf->json()["simhash"]["stop"])
    {
        _dirScanner.traverse(conf->json()["webs"]);
        _pages = _dirScanner.getFiles();

        _fingerprint.reserve(1024);
        readChineseStopWords(_stopWords);
        //cerr << "PageLib\n";
    }

string replaceAll(const std::string& str) {
    std::regex html_space_regex(u8"(\u3000|&nbsp;|&ensp;|&emsp;|&#160;|&#8203;|&#173;)");
    return std::regex_replace(str, html_space_regex, "");
}

bool PageLib::process(string fileName){
    XMLDocument doc;
    doc.LoadFile(fileName.c_str());
    XMLElement * element = doc.FirstChildElement()->FirstChildElement("channel");
    if(element == nullptr) return false;
    element = element->FirstChildElement("item");

    while(element != nullptr){
        XMLElement* titleElement = element;
        RssItem rss;
        rss.id = id;
        titleElement = titleElement->FirstChildElement("title");
        if(titleElement == nullptr) return false;

        rss.title = titleElement->GetText();
        rss.title = regex_replace(rss.title, tag_regex, "");
        
        rss.title = replaceAll(rss.title); 

        titleElement = titleElement->NextSiblingElement("link");
        if(titleElement == nullptr) return false;
        rss.link = titleElement->GetText();

        titleElement = titleElement->NextSiblingElement("description");
        if(titleElement == nullptr) return false;
        rss.description = titleElement->GetText();

        rss.description = regex_replace(rss.description, tag_regex, "");
        rss.description = replaceAll(rss.description); 

        titleElement = titleElement->NextSiblingElement("content:encoded");
        if(titleElement == nullptr){
            rss.content = "";

            if(cutRedundantPage(rss)){
                rss.description = dealChinesehWord(rss.description);
                store(rss);
                id++;
            }
            element = element->NextSiblingElement("item");
            continue;
        }

        rss.content = titleElement->GetText();
        rss.content = regex_replace(rss.content, tag_regex, "");
        rss.content = replaceAll(rss.content); 

        if(cutRedundantPage(rss)){
            rss.description = dealChinesehWord(rss.description);
            rss.content = dealChinesehWord(rss.content);
            store(rss);
            id++;
        }

        element = element->NextSiblingElement("item");
    }
    return true;
}

void PageLib::create(){
    _pages = _dirScanner.getFiles();

    for(int i = 0;i < _pages.size();i++){
        bool result = process(_pages[i]);
        if(result == false){
            cout << _pages[i] << "\n";
        }
    }

    ofstream ofs(_conf->json()["newoffset"]);
    for(const auto & offset : _offsetLib){
        ofs << offset.first << " "
        << offset.second.first << " "
        << offset.second.second << "\n";
    }
    ofs.close();
}

//存储网页
void PageLib::store(const RssItem & rss){
    ofstream ofs(_conf->json()["newripepage"],std::ios::app);

    if(!ofs){
        cerr << "error in PageLig::store\n";
        return;
    }

    int start = ofs.tellp();
    _offsetLib[rss.id].first = start;

    ofs << "<doc>\n";
    ofs << "<docid>" << rss.id << "</docid>\n";
    ofs << "<title>" << rss.title<< "</title>\n";
    ofs << "<link>" << rss.link << "</link>\n";
    ofs << "<description>" << rss.description << "</description>\n";
    ofs << "<content>" << rss.content << "</content>\n";
    ofs << "</doc>\n";
    int end = ofs.tellp();
    _offsetLib[rss.id].second = end - start - 1;

    ofs.close();

}

//去掉冗余网页(与已经存储的网页高度重复的网页)
bool PageLib::cutRedundantPage(RssItem & rss){

    string article = rss.content + rss.description;
    uint64_t u64 = 0;
    size_t topN = 20;
    _simhasher.make(article,topN,u64);

    for(int i = 0;i < _fingerprint.size();i++){
        if(Simhasher::isEqual(_fingerprint[i],u64)){
            return false;
        }
    }
    _fingerprint.push_back(u64);
    return true;
}
