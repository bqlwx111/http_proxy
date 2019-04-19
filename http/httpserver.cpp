#include <sys/socket>
#include "httpserver.h"
#include <iostream>
#include <thread>
httpServer::httpServer()
{
    _epoll_fd=epoll_create(1024);
}
httpServer::~httpServer()
{
    std::close(_epoll_fd);
}
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

    _port_to_sockfd[_port]=_sockfd;
    return 1;
}

int httpserver::addsocket()
{
    if(initSocket()==1)
    {
        epoll_event e.events=EPOLLIN|EPOLLOUT;
        epoll_ct(_epoll_fd,EPOLL_CTL_ADD,_sockfd,e);
    }
    else return -1;
}

int httpserver::run()
{
    while(true)
    {
        int events_num=epoll_wait(_epoll_fd,_MessageQueue,1024,-1);
        for(int i=0;i<events_num;i++)
        {
            thread t(handleRequest);
            t.detach();
        }

    }
}