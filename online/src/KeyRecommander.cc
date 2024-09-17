#include "../include/KeyRecommander.h"
#include "../include/TcpConnection.h"

#include <set>
#include <fstream>
#include <iostream>
#include <sstream>


using std::ofstream;
using std::ifstream;
using std::istringstream;
using std::cerr;
using std::set;
using std::cout;

Dictionary * Dictionary::dictionary = nullptr;

Dictionary * Dictionary::getInstance(){
    if(dictionary == nullptr){
        dictionary = new Dictionary();
        cout << "dictionary create\n";
    }
    return dictionary;
}

Dictionary::Dictionary(){
}

void Dictionary::init(Configuration * conf){

    ifstream ifs(conf->json()["dictIndex"]);
    if(!ifs){
        cerr << "error in init\n";
    }
    
    string line;
    while(getline(ifs,line)){
        istringstream iss(line);
        int first = 0;
        string input;
        string word;
        while(iss >> input){
            if(first == 0){
                word = input;
                first++;
                continue;
            }
            _index[word].insert(atoi(input.c_str()));
        }   
    }
    ifs.close();

    ifs.open(conf->json()["ChineseDict"]);
    while(getline(ifs,line)){
        istringstream iss(line);

        string word;
        string freq;
        iss >> word;
        iss >> freq;
        _ChineseDict.push_back({word,atoi(freq.c_str())});
    }
    ifs.close();

    ifs.open(conf->json()["EnglishDict"]);

    while(getline(ifs,line)){
        istringstream iss(line);
        string word;
        string freq;
        iss >> word;
        iss >> freq;
        _EnglishDict.push_back({word,atoi(freq.c_str())});
    }
    ifs.close();
}

map<string,set<int>> & Dictionary::getIndexTable(){
    return _index;
}

vector<pair<string,int>> & Dictionary::getEnglishDict(){
    return _EnglishDict;
}

vector<pair<string,int>> & Dictionary::getChineseDict(){
    return _ChineseDict;
}

size_t nBytesCode(const char ch){
    if(ch & (1 << 7)){
        int nBytes = 1;

        for(int idx = 0;idx != 6;++idx){
            if(ch & (1 << (6 - idx))){
                ++nBytes;
            }else{
                break;
            }
        }
        return nBytes;
    }
    return 1;
}

vector<pair<string,int>> KeyRecommander::cutWords(){
    vector<pair<string,int>> characters;
    for(size_t i = 0;i < _queryWord.size();i++){
        char c = _queryWord[i];

        size_t bytes = nBytesCode(c);
        characters.push_back({_queryWord.substr(i,bytes),bytes});
        i += (bytes - 1);
    }
    return characters;
}

int triple_min(const int &a, const int &b, const int &c)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

std::size_t length(const std::string &str)
{
	std::size_t ilen = 0;
	for(std::size_t idx = 0; idx != str.size(); ++idx)
	{
		int nBytes = nBytesCode(str[idx]);
		idx += (nBytes - 1);
		++ilen;
	}
	return ilen;
}

int KeyRecommander::distance(const string &rhs)
{
    string lhs = _queryWord;
	size_t lhs_len = length(lhs);
	size_t rhs_len = length(rhs);
	int editDist[lhs_len + 1][rhs_len + 1];
	for(size_t idx = 0; idx <= lhs_len; ++idx)
	{
		editDist[idx][0] = idx;
	}

	for(size_t idx = 0; idx <= rhs_len; ++idx)
	{
		editDist[0][idx] = idx;
	}
	
	std::string sublhs, subrhs;
	for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
	{
		size_t nBytes = nBytesCode(lhs[lhs_idx]);
		sublhs = lhs.substr(lhs_idx, nBytes);
		lhs_idx += (nBytes - 1);

		for(std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
		{
			nBytes = nBytesCode(rhs[rhs_idx]);
			subrhs = rhs.substr(rhs_idx, nBytes);
			rhs_idx += (nBytes - 1);
			if(sublhs == subrhs)
			{
				editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
			}
			else
			{
				editDist[dist_i][dist_j] = triple_min(
					editDist[dist_i][dist_j - 1] + 1,
					editDist[dist_i - 1][dist_j] + 1,
					editDist[dist_i - 1][dist_j - 1] + 1);
			}
		}
	}
	return editDist[lhs_len][rhs_len];
}


void KeyRecommander::execute(){
    vector<pair<string,int>> words = cutWords();

    set<int> candidateEnglishSet;
    set<int> candidateChineseSet;
    set<pair<string,int>> result;

    map<string,set<int>> & index = _dictionary->getIndexTable();
    vector<pair<string,int>> & ChineseDict = _dictionary->getChineseDict();
    vector<pair<string,int>> & EnglishDict = _dictionary->getEnglishDict();

    for(const auto & wordInfo : words){
        string word = wordInfo.first;
        int bytes = wordInfo.second;
        
        if(bytes == 1) word[0] = tolower(word[0]);

        set<int> candidatesId = index[word];
        // for(const auto & ele : candidatesId){
        //     cout << ele << " ";
        // }
        if(bytes == 1){
            candidateEnglishSet.insert(candidatesId.begin(),candidatesId.end());
        }else{
            candidateChineseSet.insert(candidatesId.begin(),candidatesId.end());
        }
    }

    for(const int & ele : candidateEnglishSet){
        result.insert(EnglishDict[ele]);
    }

    for(const int & ele : candidateChineseSet){
        result.insert(ChineseDict[ele]);
    }
    CandidateResult cr;

    for(const auto & p : result){
        string cand = p.first;
        int dist = distance(cand);
        cr._dist = dist;
        cr._word = cand;
        cr._freq = p.second;
        _prique.push(cr);
    }
}

void KeyRecommander::response(TcpConnectionPtr & tcp){
    
    nlohmann::json respJs;

    for(int i = 0;i < 10;i++){
        if(_prique.size() == 0) break;
        respJs["suggestions"].push_back(_prique.top()._word);
        _prique.pop();
    }
    //cout << respJs.dump() << "\n";
    tcp->sendInLoop(respJs.dump());
}
