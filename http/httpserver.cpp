#include <sys/socket>
#include "httpserver.h"
#include "httpRequest.h"
#include <iostream>
#include <thread>
httpServer::httpServer()
{
    _epoll_fd=epoll_create(1024);
}
httpServer::~httpServer()
{
    for(auto& i:_port_to_sockfd)
        std::close(i);

    std::close(_epoll_fd);
}

int httpserver::initSocket()
{
    _sockfd=socket(AF_INET,SOCK_STREAM,SOCK_STREAM|SOCK_NONBLOCK,IPPROTO_TCP);
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
    return _sockfd;
}

int httpserver::addsocket()
{
    if(initSocket()!=-1)
    {
        epoll_event e.events=EPOLLIN|EPOLLOUT;
        e.data.fd=_sockfd;
        epoll_ct(_epoll_fd,EPOLL_CTL_ADD,_sockfd,e);
    }
    else return -1;
}

int ComingSokcet(int& listenFd)
{
    struct sockaddr_in clientAddr;
    socklen_t clilen;
    int sock_fd=accept(listenFd,(sockaddr* )&clientAdd,clilen);

    epoll_event e;
    e.events=EPOLLIN|EPOLLOUT|EPOLLHUP;
    e.data.fd=sock_fd;
    epoll_ct(_epoll_fd,EPOLL_CTL_ADD,sock_fd,e);

}

//====================================
typedef std::shared_ptr<httpRequest> Request;
typedef std::shared_ptr<httpResponse> Response;

int ReadSocket(epoll_event & readableEvent)
{
    Request request(new httpRequest);
    std::string request_string(recv());
    handleRequest(request_string);

    epoll_event e;
    e.events=EPOLLOUT;


    e.ptr=&request;//todo


    epoll_ct(_epoll_fd,EPOLL_CTL_MOD,readableEvent.data.fd,e);
}

int WriteSocket(epoll_event& writeableEvent)
{
    parseRequest();
    Response response (new httpResponse);
    std::string type;
    if(type=="get")
    {
        get();
        return;
    }
    if(type=="post")
    {
        post();
        return;
    }
    if(type=="head")
    {
        head();
        return;
    }
    if(type=="put")
    {
        head();
        return;
    }
    if(type=="not implemented")
    {
        not_implemented();
        return;
    }
}
//========================================================
int CloseSokcet(epoll_event& closeAbleEvent)
{
    closeAbleEvent.data.ptr=NULL;
    Close(closeAbleEvent.data.fd);
}

int httpserver::run()
{
    while(true)
    {
        int events_num=epoll_wait(_epoll_fd,_MessageQueue,1024,-1);
        for(int i=0;i<events_num;i++)
        {
            if(_port_to_sockfd.find(_MessageQueue[i].data.fd)!=_port_to_sockfd.end())
            { 
                thread t(ComingSokcet,_MessageQueue[i].data.fd);
                t.detach();
                continue;
            }

            if(_MessageQueue[i].events&EPOLLIN)
            {
                thread t(ReadSocket,_MessageQueue[i]);
                t.detach();
                continue;
            }

            if(_MessageQueue[i].events&EPOLLOUT)
            {
                thread t(WriteSocket);
                t.detach();
                continue;
            }

            if(_MessageQueue[i].events&EPOLLHUP)
            {
                thread t(CloseSokcet);
                t.detach();
                continue;
            }
            
        }

    }
}