#include "../include/DictProducer.h"
#include "../include/SplitToolCppJieba.h"
#include "../include/DirScanner.h"

#include <iostream>
using std::cout;
using std::cin;
using std::endl;


int main() {

    DirScanner scan;
    Configuration * conf = Configuration::getInstance();

    SplitTool * tool = SplitToolJieba::getIntance(*conf);

    scan.traverse(conf->json()["ChineseYuliao"]);
    vector<string> files = scan.getFiles();


    DictProducer ChineseFile(*conf,files,tool);

    ChineseFile.buildCnDict();
    ChineseFile.createChineseIndex();
    ChineseFile.storeChineseDict();

    vector<string> engFiles;
    engFiles.push_back(conf->json()["EnglishYuliao"]);
    DictProducer EnglishFile(*conf,engFiles,tool);

    EnglishFile.buildEnDict();
    EnglishFile.createEnglishIndex();
    EnglishFile.storeEnglishDict();

    return 0;
}