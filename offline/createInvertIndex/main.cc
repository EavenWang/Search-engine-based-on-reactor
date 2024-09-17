#include "invertIndex.h"
#include "../include/SplitTool.h"
#include "../include/SplitToolCppJieba.h"
#include "../include/Configuration.h"

#include <iostream>
#include <unordered_set>
#include <fstream>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::unordered_set;
using std::ifstream;
using std::ofstream;


int main() {

    Configuration * conf = Configuration::getInstance();

    SplitTool * tool = SplitToolJieba::getIntance(*conf);

    InvertIndex ivt(tool,conf);

    cerr << "start\n";
    
    ivt.processDoc(conf->json()["newripepage"]);
    ivt.computeWeight();
    
    ivt.store(conf->json()["invertIndex"]);
    conf->destroy();

    return 0;
}



