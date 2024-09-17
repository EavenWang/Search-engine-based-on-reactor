#include "invertIndex.h"
#include "../../library/tinyxml/tinyxml2.h"

#include <iostream>
#include <fstream>
#include <math.h>

using std::ifstream;
using std::ofstream;
using std::cerr;

using namespace tinyxml2;

string dealChinesehWord(const string &word)
{
    string newWord;
    for (size_t idx = 0; idx != word.size(); idx++)
    {   
        if (word[idx] != '\r' && word[idx] != '\n' && word[idx] != ' ')
        {
            newWord += word[idx];
        }
    }
    return newWord;
}

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


void InvertIndex::processDoc(string filename){
    XMLDocument doc;
    doc.LoadFile(filename.c_str());
    XMLElement * element = doc.FirstChildElement("doc");

    while(element != nullptr){

        string sentence = "";

        XMLElement * child = element->FirstChildElement("docid");
        
        int id;
        if(child != nullptr) id = atoi(child->GetText());
        else return;

        _numOfArticles++;
        child =  element->FirstChildElement("title");

        if(child != nullptr) sentence = sentence + child->GetText() + "。";

        child = element->FirstChildElement("description");

        if(child != nullptr){
            if(child->GetText()){
                sentence = sentence + child->GetText();
            }
        }

        child = element->FirstChildElement("content");

        if(child != nullptr){
            if(child->GetText()){
                sentence = sentence + child->GetText();
            }
        }

        sentence = dealChinesehWord(sentence);
        computeSingleWeight(id,sentence);
        element = element->NextSiblingElement("doc");
    }
}

void InvertIndex::computeSingleWeight(int id,string & sentence){
    vector<string> cutResult =  _tool->cut(sentence);

    unordered_map<string,int> freq;

    for(const string & word : cutResult){
        if(nBytesCode(word[0]) > 1 && _stopWords.find(word) == _stopWords.end())
            freq[word]++;
    }

    for(const auto & ele : freq){
        _wordsFrequency[ele.first].insert({id,ele.second});
    }
}

void InvertIndex::computeWeight(){

    double weight,TF,DF,IDF;


    unordered_set<string> stopWords;
    
    

    int docid;
    for(const auto & ele : _wordsFrequency){
        string word = ele.first;
        

        for(const auto & p : ele.second){
            docid = p.first;
            TF = p.second;
            DF = ele.second.size();
            IDF = log2(_numOfArticles /(DF + 1) + 1);
            weight = TF * IDF;

            _articleWeights[docid] += weight * weight;
            _invertIndexTable[word].insert({docid,weight});
        }
    }

    //归一化
    for(auto & invertWord : _invertIndexTable){
        string wordInTable = invertWord.first;
        set<pair<int,double>> temp;

        for(auto & pairInSet : invertWord.second){
            int id = pairInSet.first;
            double weigthOfWord = pairInSet.second;
            weigthOfWord = weigthOfWord / (sqrt(_articleWeights[id]));
            temp.insert({id,weigthOfWord});
        }
        _invertIndexTable[wordInTable] = temp;
    }
}

void InvertIndex::store(string filename){
    ofstream ofs(filename);
    if(!ofs){
        cerr << "error in buildInvertIndexMap\n";
    }

    for(const auto & invertWord : _invertIndexTable){
        string word = invertWord.first;
        ofs << word << " ";
        for(const auto & pairInSet : invertWord.second){
                ofs << pairInSet.first << " "
                << pairInSet.second << " ";
        }
        ofs << "\n";
    }
    ofs.close();
}

void InvertIndex::readChineseStopWords(unordered_set<string> & stopWordsSet){

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