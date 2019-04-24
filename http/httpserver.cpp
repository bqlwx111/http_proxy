#include <sys/socket.h>

#include "httpserver.h"

#include "parse.hxx"

#include "httpRequest.h"
#include <iostream>
#include <thread>

#include<unistd.h>

httpServer::httpServer()
{
    _epoll_fd=epoll_create(1024);
}
httpServer::~httpServer()
{
    for(auto& i:_port_to_sockfd)
        close(i.second);

    close(_epoll_fd);
}

int httpServer::handleRequest(std::string request)
{

}

httpServerReturnType httpServer::get()
{

}
httpServerReturnType httpServer::put()
{

}
httpServerReturnType httpServer::head()
{

}
httpServerReturnType httpServer::post()
{

}
httpServerReturnType httpServer::not_implemented()
{
    
}

int httpServer::initSocket()
{
    _sockfd=socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,IPPROTO_TCP);
    if(_sockfd<0)
    {
        std::cout<<"Failed to create socket"<<std::endl;
        return -1;
    }

    _servAddr.sin_family  =AF_INET;
    _servAddr.sin_port = _port;
    _servAddr.sin_addr.s_addr=INADDR_ANY;

    int status;
    status=bind(_sockfd,(sockaddr *)&_servAddr,sizeof(_servAddr));
    if(status<0)  
    {
        std::cout<<"Failed to bind socket to port: "<<_port<<std::endl;
        return -1;
    }

    status=listen(_sockfd,20);
    if(status)
    {
        std::cout<<"Listen on port "<<_port<<" failed"<<std::endl;
		return -1;
    }
    std::cout<<"httpserver listen on port "<<_port<<" successful"<<std::endl;

    _port_to_sockfd[_port]=_sockfd;
    return _sockfd;
}

int httpServer::addsocket()
{
    if(initSocket()!=-1)
    {
        epoll_event e;
        e.events=EPOLLIN|EPOLLOUT;
        e.data.fd=_sockfd;
        epoll_ctl(_epoll_fd,EPOLL_CTL_ADD,_sockfd,&e);
    }
    else return -1;
}

int httpServer::ComingSokcet(epoll_event& ComingEvent)
{
    //struct sockaddr_in clientAddr;
    struct sockaddr clientAddr;
    socklen_t clilen;
    int sock_fd=accept(ComingEvent.data.fd,(sockaddr* )&clientAddr,&clilen);

    epoll_event e;
    e.events=EPOLLIN|EPOLLOUT|EPOLLHUP;
    e.data.fd=sock_fd;
    epoll_ctl(_epoll_fd,EPOLL_CTL_ADD,sock_fd,&e);

    return 1;
}

//====================================
typedef std::shared_ptr<httpRequest> Request;
typedef std::shared_ptr<httpResponse> Response;

int httpServer::ReadSocket(epoll_event & readableEvent)
{
    Request request(new httpRequest);
    //std::string request_string(recv());
    std::string request_string;
    handleRequest(request_string);

    epoll_event e;
    e.events=EPOLLOUT;
    e.data.ptr=&request;//todo


    epoll_ctl(_epoll_fd,EPOLL_CTL_MOD,readableEvent.data.fd,&e);

    return 1;
}

int httpServer::WriteSocket(epoll_event& writeableEvent)
{
    //headerParse::FindField("abc");//todo
    Response response (new httpResponse);
    std::string type;
    if(type=="get")
    {
        get();
        return 1;
    }
    if(type=="post")
    {
        post();
        return 1;
    }
    if(type=="head")
    {
        head();
        return 1;
    }
    if(type=="put")
    {
        head();
        return 1;
    }
    if(type=="not implemented")
    {
        not_implemented();
        return 1;
    }

    return 1;
}
//========================================================
int httpServer::CloseSokcet(epoll_event& closeAbleEvent)
{
    closeAbleEvent.data.ptr=NULL;
    close(closeAbleEvent.data.fd);
    return 1;
}

int httpServer::run()
{
    while(true)
    {
        int events_num=epoll_wait(_epoll_fd,_MessageQueue,1024,-1);
        for(int i=0;i<events_num;i++)
        {
            if(_port_to_sockfd.find(_MessageQueue[i].data.fd)!=_port_to_sockfd.end())
            { 
                std::thread t(&(httpServer::ComingSokcet),this,std::ref(_MessageQueue[i]));
                t.detach();
                continue;
            }

            if(_MessageQueue[i].events&EPOLLIN)
            {
                std::thread t(&(httpServer::ReadSocket),this,std::ref(_MessageQueue[i]));
                t.detach();
                continue;
            }

            if(_MessageQueue[i].events&EPOLLOUT)
            {
                std::thread t(&(httpServer::WriteSocket),this,std::ref(_MessageQueue[i]));
                t.detach();
                continue;
            }

            if(_MessageQueue[i].events&EPOLLHUP)
            {
                std::thread t(&(httpServer::CloseSokcet),this,std::ref(_MessageQueue[i]));
                t.detach();
                continue;
            }
            
        }

    }
}