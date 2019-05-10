#include <sys/socket.h>

#include "httpserver.h"

#include "parse.hxx"

#include "httpRequest.h"
#include "httpResponse.h"
#include <mutex>


#include <iostream>
#include <thread>
#include <cstring>

#include<unistd.h>

#include <errno.h>
std::mutex mut;

struct fd_request
{
    httpRequest* request;
    int fd;

    fd_request()
    {
    }
    
    ~fd_request()
    {
    }
};


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

void httpServer::get(Request& request,Response& response)
{
    response->setStatusCode(200);
    response->setVersion(HTTP1_0);

    response->addHttpHeader(std::string("Cache-Control"),std::string("private, max-age=0"));
    response->addHttpHeader(std::string("Content-Encoding"),std::string("application/json"));
    response->setResponseBody(std::string("{ \"c++\": \"std11\"};"));

    response->setResponse();
}

void httpServer::put(Request& request,Response& response)
{

}
void httpServer::head(Request& request,Response& response)
{

}
void httpServer::post(Request& request,Response& response)
{
    response->setStatusCode(200);
    response->setVersion(HTTP1_0);

    response->addHttpHeader(std::string("Cache-Control"),std::string("private, max-age=0"));
    response->addHttpHeader(std::string("Content-Encoding"),std::string("application/json"));
    response->setResponseBody(std::string("{ \"c++\": \"std11\"};"));

    response->setResponse();

}
void httpServer::not_implemented(Request& request,Response& response)
{
    
}

int httpServer::initSocket()
{
    _sockfd=socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,IPPROTO_TCP);
    if(_sockfd<0)
    {
        std::cout<<"104 :Failed to create socket"<<std::endl;
        return -1;
    }

    _servAddr.sin_family  =AF_INET;
    _servAddr.sin_port = htons(_port);
    _servAddr.sin_addr.s_addr=INADDR_ANY;

    int status;
    status=bind(_sockfd,(sockaddr *)&_servAddr,sizeof(_servAddr));

    if(status!=0)  
    {
        std::cout<<"117: Failed to bind socket to port: "<<_port<<std::endl;
        return -1;
    }

    status=listen(_sockfd,20);

    if(status)
    {
        std::cout<<"125: Listen on port "<<_port<<" failed"<<std::endl;
		return -1;
    }
    std::cout<<"128: httpserver listen on : "<<_ip<<" :: "<<_port<<" successful"<<std::endl;

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
        e.data.ptr=new fd_request;
        fd_request* fdRequest=(fd_request*)e.data.ptr;

        e.events=EPOLLIN|EPOLLOUT|EPOLLET;
        fdRequest->fd=_sockfd;

        std::cout<<"147 : addsocket()"<<std::endl;
        epoll_ctl(_epoll_fd,EPOLL_CTL_ADD,_sockfd, &e);

    }
    else return -1;
}

int httpServer::ComingSocket(epoll_event& ComingEvent)
{
    //struct sockaddr_in clientAddr;
    struct sockaddr clientAddr;
    socklen_t clilen;
    fd_request* fdRequest=(fd_request*)ComingEvent.data.ptr;

    int sock_fd=accept4(fdRequest->fd,(sockaddr* )&clientAddr,&clilen,SOCK_NONBLOCK);
   
    std::cout<<"163: ComingSocket::sock_fd :"<<sock_fd<<" epolled"<<" fdRequest->fd :"<<fdRequest->fd<<std::endl;

    epoll_event e;

    e.events=EPOLLIN|EPOLLHUP|EPOLLET;

    e.data.ptr=new fd_request;
    fdRequest=(fd_request*)e.data.ptr;
    fdRequest->fd=sock_fd;


    epoll_ctl(_epoll_fd,EPOLL_CTL_ADD,sock_fd,&e);

    //e.events=EPOLLIN|EPOLLET;
    //epoll_ctl(_epoll_fd,EPOLL_CTL_MOD,sock_fd,&e);


    return 1;
}

//====================================
typedef std::shared_ptr<httpRequest> Request;
typedef std::shared_ptr<httpResponse> Response;

int httpServer::ReadSocket(epoll_event & readableEvent)
{
    fd_request* fdRequest=(fd_request*)readableEvent.data.ptr;

    std::cout<<"191: ReadSocket fd: "<<fdRequest->fd<<std::endl;
    std::string request_string;

    char buf[32];

    int n=0;

    while(true) //http client only send one message in each request
    {
        n=recv(fdRequest->fd, buf,32, 0);
        if(n<=0)
        {
            //for http client only send one message each time ,we will handle EWOULDBLOCK AND EAGAIN  as break
            if((errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))//g++ provide thread security for errno
                break;
            else
            {
                epoll_ctl(_epoll_fd,EPOLL_CTL_DEL,fdRequest->fd,&readableEvent);
                std::cout<<"208 readableEvent.data.fd "<<fdRequest->fd<<std::endl;
                delete (fd_request*)readableEvent.data.ptr;
                readableEvent.data.ptr=NULL;
                close(readableEvent.data.fd);
                break;
            }
        }

        std::string temp;
        temp.resize(n);
        for(int i=0;i<n;i++)
        {
            temp[i]=buf[i];
            buf[i]=0;
        }
        request_string+=temp;
    }

    int pos=-1;


    while((pos=request_string.find("\r\n"))!=request_string.npos)
    {
        request_string.replace(pos,2,"\n");
    }
    



    /*
    typedef union epoll_data 
    {
        void *ptr;
        int fd;
        __uint32_t u32;
        __uint64_t u64;
    } epoll_data_t;

*/

    
    readableEvent.events=EPOLLOUT|EPOLLHUP|EPOLLET;
    fdRequest->request=new httpRequest;
    fdRequest->request->parseRequest(request_string);



    epoll_ctl(_epoll_fd,EPOLL_CTL_MOD,fdRequest->fd,&readableEvent);
;

    return 1;
}

int httpServer::WriteSocket(epoll_event& writeableEvent)
{
    //headerParse::FindField("abc");//todo
    fd_request* fdRequest=(fd_request*)writeableEvent.data.ptr;

    Response response(new httpResponse);

    std::cout<<"269 WriteSocket::event called : eventfd:"<<fdRequest->fd<<std::endl;
    Request request (fdRequest->request);
    std::cout<<"http request==nullptr ? "<<(fdRequest->request==nullptr)<<std::endl;

    
    if(request->getMethod()==GET)
    {
        std::cout<<"get()"<<std::endl;
        get(request,response);
    }

    if(request->getMethod()==POST)
    {
        std::cout<<"post()\n";
        post(request,response);
    }

    if(request->getMethod()==HEAD)
    {
        head(request,response);
    }

    if(request->getMethod()==PUT)
    {
        put(request,response);
    }

    if(request->getMethod()==NOT_IMPLEMENTED)
    {
        not_implemented(request,response);
    }

    response->showResponse();
    std::string response_string=response->getResponse();
    

    long  stringsize;   

    stringsize=response_string.size();


    char buf[1024*1024];
    if(stringsize>(1024*1024))
        std::cout<<"314 detected large file more than 1024*1024 bytes "<<std::endl;

    for(int i=0;i<stringsize;i++)
        buf[i]=response_string[i];
    
    int n=0;
    int counter=0;
    while(true)
    {
        n=send(fdRequest->fd,buf,stringsize,0);



        if(n<=0)
        {
            if(n==0)
            {   
                std::cout<<"3"<<std::endl;
                epoll_ctl(_epoll_fd,EPOLL_CTL_DEL,fdRequest->fd,&writeableEvent);
                close(fdRequest->fd);
                break;
            }
            if(errno==EAGAIN)
            {
                std::cout<<"4"<<std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                continue;
            }
            std::cout<<"5"<<std::endl;   
            break;
        }
        counter+=n;
        if(counter==stringsize)
            break;
    }
//==================================close connection 
    std::cout<<"closeConnection()";
    std::cout<< "fd: "<<fdRequest->fd<<std::endl;
    epoll_ctl(_epoll_fd,EPOLL_CTL_DEL,fdRequest->fd,&writeableEvent);
    close(fdRequest->fd);
    delete fdRequest;

    // not modified the fd status 
    // core dumped here
    // shared_ptr cause
    // double free 
    return 1;
}
//========================================================
int httpServer::OnHup(epoll_event& closeAbleEvent)
{
    std::cout<<"363: OnHup()";
    fd_request* fdRequest=(fd_request*)closeAbleEvent.data.ptr;
    std::cout<< "365 hup->fd: "<<fdRequest->fd<<std::endl;
    epoll_ctl(_epoll_fd,EPOLL_CTL_DEL,fdRequest->fd,&closeAbleEvent);
    close(fdRequest->fd);
    delete fdRequest->request;
    delete fdRequest;
    return 1;
}

int httpServer::run()
{
    char n;
    while(true)
    {
        int events_num=epoll_wait(_epoll_fd,_MessageQueue,1024,-1);//
    
        for(int i=0;i<events_num;i++)
        {
            fd_request* fdRequest=(fd_request*)_MessageQueue[i].data.ptr;

            if(_MessageQueue[i].events&EPOLLHUP)
            {
                std::thread t(&(httpServer::OnHup),this,std::ref(_MessageQueue[i]));
                t.detach();
                continue;
            }

            if(_listen_fds.find(fdRequest->fd)!=_listen_fds.end())
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

            
        }

    }
}