#include "../include/Server.h"



int main(int argc, char **argv)
{
    myServer server(4, 10,"192.168.44.129", 1234);    

    server.start();
    return 0;
}
