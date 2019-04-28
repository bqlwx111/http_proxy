#include <memory>
#include <chrono>
#include <thread>
#include <string>
#include <iostream>
#include "httpserver.h"
using namespace std;
int main()
{
    unique_ptr<httpServer> server (new httpServer);
    
    server->setIp("0.0.0.0");
    server->setPort(8000);
    server->addsocket();

    server->setIp("0.0.0.0");
    server->setPort(8001);
    server->addsocket();

    server->setIp("0.0.0.0");
    server->setPort(8002);
    server->addsocket();

    server->setIp("0.0.0.0");
    server->setPort(8003);
    server->addsocket();

    server->run();
}