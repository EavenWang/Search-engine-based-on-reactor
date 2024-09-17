#include "../include/DirScanner.h"
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>

using std::cerr;
using std::ifstream;
using std::cout;

vector<string> & DirScanner::getFiles(){
    return _files;
}

void DirScanner::traverse(string dir){


    DIR * dirPath = opendir(dir.c_str());

    if(dirPath == nullptr){
        cerr << "error in DirScanner::traverse\n";
    }

    struct  dirent * ptr;

    while((ptr = readdir(dirPath)) != nullptr){
        if(strcmp(ptr->d_name,".") == 0 || strcmp(ptr->d_name,"..") == 0){
            continue;
        } 
        string file = dir + "/" + ptr->d_name;
        _files.push_back(file);
    }
}