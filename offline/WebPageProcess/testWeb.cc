#include "../include/PageLib.h"
#include "../include/Configuration.h"

#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::cerr;


int main() {

    Configuration * conf = Configuration::getInstance();

    DirScanner scann;
    PageLib pages(conf,scann);

    pages.create();
    
    return 0;
}