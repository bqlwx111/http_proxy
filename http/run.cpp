#include "httpserver.h"
#include <memory>
#include <chrono>
using namespace std;
int main()
{
    unique_ptr<httpServer> server (new httpServer);
    while(true)
    {
        this_thread::sleep_for(chrono::seconds(1));
    } 
}