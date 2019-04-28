#include <sys/socket.h>

#include "httpserver.h"

#include "parse.hxx"

#include "httpRequest.h"
#include "httpResponse.h"


#include <iostream>
#include <thread>
#include <cstring>

#include<unistd.h>

#include <errno.h>


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
typedef std::shared_ptr<httpRequest> Request;
typedef std::shared_ptr<httpResponse> Response;

/*
Cache-Control →private, max-age=0
Content-Length →46225
Content-Type →text/html; charset=utf-8
Content-Encoding →gzip
Vary →Accept-Encoding
*/

Response& httpServer::get(Request&)
{
    Response response (new httpResponse);
    response->setStatusCode(200);
    response->setVersion(HTTP1_0);

    response->addHttpHeader(std::string("Cache-Control"),std::string("private, max-age=0"));
    response->addHttpHeader(std::string("Content-Type"),std::string("text/html; charset=utf-8"));
    response->addHttpHeader(std::string("Content-Encoding"),std::string("application/json"));
    response->setResponseBody(std::string("{ \"c++\": \"std11\"};"));

    response->addHttpHeader(std::string("Content-Length"),std::string("500"));

    //response->setResponse();

    return response;
}


Response& httpServer::put(Request&)
{

}
Response& httpServer::head(Request&)
{
}
Response& httpServer::post(Request&)
{

}
Response& httpServer::not_implemented(Request&)
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
    _servAddr.sin_port = htons(_port);
    _servAddr.sin_addr.s_addr=INADDR_ANY;

    int status;
    status=bind(_sockfd,(sockaddr *)&_servAddr,sizeof(_servAddr));

    if(status!=0)  
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
    std::cout<<"httpserver listen on : "<<_ip<<" :: "<<_port<<" successful"<<std::endl;

    _port_to_sockfd[_port]=_sockfd;
    _listen_fds.insert(_sockfd);
    std::cout<<"listen_fd :"<<_sockfd<<std::endl;
    return _sockfd;
}

int httpServer::addsocket()
{
    if(initSocket()!=-1)
    {
        epoll_event e;
        e.events=EPOLLIN|EPOLLOUT;
        e.data.fd=_sockfd;
        epoll_ctl(_epoll_fd,EPOLL_CTL_ADD,_sockfd, &e);
    }
    else return -1;
}

int httpServer::ComingSocket(epoll_event& ComingEvent)
{
    //struct sockaddr_in clientAddr;
    struct sockaddr clientAddr;
    socklen_t clilen;

    int sock_fd=accept4(ComingEvent.data.fd,(sockaddr* )&clientAddr,&clilen,SOCK_NONBLOCK);

    //std::cout<<"ComingSocket::sock_fd :"<<sock_fd<<" epoll successful"<<std::endl;

    epoll_event e;

    e.events=EPOLLIN|EPOLLOUT|EPOLLHUP;
    e.data.fd=sock_fd;

    epoll_ctl(_epoll_fd,EPOLL_CTL_ADD,sock_fd,&e);

    e.events=EPOLLIN;
    epoll_ctl(_epoll_fd,EPOLL_CTL_MOD,sock_fd,&e);

    return 1;
}

//====================================
typedef std::shared_ptr<httpRequest> Request;
typedef std::shared_ptr<httpResponse> Response;

int httpServer::ReadSocket(epoll_event & readableEvent)
{
    //std::string request_string(recv());
    std::string request_string;

    char buf[32];

    int n=0;
    //int a;
    while(true) //http client only send one message in each request
    {
        n=recv(readableEvent.data.fd, buf,32, 0);
        if(n<=0)
        {
            //for http client only send one message each time ,we will handle EWOULDBLOCK AND EAGAIN  as break
            if((errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))//g++ provide thread security for errno
                break;
            else
            {
                std::cout<<"called"<<std::endl;
                readableEvent.data.ptr=NULL;
                epoll_ctl(_epoll_fd,EPOLL_CTL_DEL,readableEvent.data.fd,&readableEvent);
                close(readableEvent.data.fd);
                break;
            }
        }

        //std::cout<<"bytenum:"<<n<<std::endl;
        std::string temp;
        temp.resize(n);
        for(int i=0;i<n;i++)
        {
            temp[i]=buf[i];
            buf[i]=0;
        }
        request_string+=temp;
        //if(n<32)
        //std::cout<<buf<<std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    int pos=-1;
    while((pos=request_string.find("\r\n"))!=request_string.npos)
    {
        request_string.replace(pos,2,"\n");
    }
    
    //std::cout<<"counter::"<<counter<<std::endl;
    //std::cout<<"size::"<<request_string.size()<<std::endl;
    //std::cout<<"request:: \n"<<request_string<<std::endl;
    //std::cout<<"ReadSocket::event called : eventfd:"<< readableEvent.data.fd<<std::endl;

    //Request request(new httpRequest);

    epoll_event e;
    e.events=EPOLLOUT;

    e.data.ptr=(httpRequest*)new httpRequest;
    httpRequest* request=e.data.ptr;
    request->parseRequest(request_string);


    epoll_ctl(_epoll_fd,EPOLL_CTL_MOD,readableEvent.data.fd,&e);

    return 1;
}

int httpServer::WriteSocket(epoll_event& writeableEvent)
{
    //headerParse::FindField("abc");//todo
    Response response (new httpResponse);
    //std::cout<<"WriteSocket::event called : eventfd:"<< writeableEvent.data.fd<<std::endl;

    Request request ((httpRequest*)writeableEvent.data.ptr);

    //epoll_ctl(_epoll_fd,EPOLL_CTL_MOD,readableEvent.data.fd,&e);
    if(request->getMethod()==GET)
    {
        get(request);
        return 1;
    }
    if(request->getMethod()==POST)
    {
        post(request);
        std::cout<<"post()"<<std::endl;
        return 1;
    }
    if(request->getMethod()==HEAD)
    {
        head(request);
        return 1;
    }
    if(request->getMethod()==PUT)
    {
        head(request);
        return 1;
    }
    if(request->getMethod()==NOT_IMPLEMENTED)
    {
        not_implemented(request);
        return 1;
    }

    // not modified the fd status 
    // core dumped here
    // shared_ptr cause
    // double free 
    return 1;
}
//========================================================
int httpServer::CloseSokcet(epoll_event& closeAbleEvent)
{
    closeAbleEvent.data.ptr=NULL;
    epoll_ctl(_epoll_fd,EPOLL_CTL_DEL,closeAbleEvent.data.fd,&closeAbleEvent);
    close(closeAbleEvent.data.fd);
    return 1;
}

int httpServer::run()
{
    char n;
    while(std::cin>>n)
    {
        if(n=='q')
        {
            for(auto&i:_listen_fds)
                close(i);
            break;
        }
        int events_num=epoll_wait(_epoll_fd,_MessageQueue,1024,-1);
        std::cout<<"events_num:: "<<events_num<<std::endl;
        for(int i=0;i<events_num;i++)
        {
            if(_listen_fds.find(_MessageQueue[i].data.fd)!=_listen_fds.end())
            { 
                std::thread t(&(httpServer::ComingSocket),this,std::ref(_MessageQueue[i]));
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