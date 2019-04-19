#include <sys/socket>
#include "httpserver.h"
#include <iostream>

int httpserver::initSocket()
{
    _sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(_sockfd<0)
    {
        std::cout<<"Failed to create socket"<<std::endl;
        return -1;
    }

    _servAddr.sin_family  =AF_INET;
    _servAddr.sin_port = _port;
    _servAddr.sin_addr.s_addr=INADDR_ANY;

    int status;
    status=bind(_sockfd,(sockaddr *)&servAddr,sizeof(_servAddr));
    if(status<0)  
    {
        std::cout<<"Failed to bind socket to port: "<<_port<<std::endl;
        return -1;
    }

    status=listen(_sockfd,20);
    if(status)
    {
        std::cout<<"Listen on port "<<svrPort<<" failed"<<std::endl;
		return -1;
    }
    std::cout<<"httpserver listen on port "<<svrPort<<" successful"<<std::endl;
    return 1;
}
int httpserver::run()
{
    while(true)
    {
        _have_event
    }
}