#include <string>
#include "httpRequest.h"
#include "httpResponse.h"


#include<sys/epoll.h>
#include<sys/socket.h>

#include <queue>
#include <unordered_map>
#include <netinet/in.h>
#include <set>
struct  httpServerReturnType
{
    
};

class httpServer
{
    public:
        httpServer();
        virtual ~httpServer();
//==============================================
        virtual int run ();

        void setUrl(std::string url){_url=url;}
        void setPort(int port){_port=port;}
        void setIp(std::string ip){_ip=ip;}

        int getEpollfd(){return _epoll_fd;}
        epoll_event* getMessageQueue(){return _MessageQueue;}


        //GET, PUT, HEAD, POST, NOT_IMPLEMENTED
        virtual httpServerReturnType get();
        virtual httpServerReturnType put();
        virtual httpServerReturnType head();
        virtual httpServerReturnType post();
        virtual httpServerReturnType not_implemented();
//===========================================================

        int addsocket();
        int initSocket();
        int ReadSocket(epoll_event &);
        int WriteSocket(epoll_event&);
        int ComingSocket(epoll_event& );
        int CloseSokcet(epoll_event&);

        int handleRequest(std::string );

    private:
        std::string _ip;
        std::string _url;
        int _port;
        int _epoll_fd;

        sockaddr_in _servAddr;
        //sockaddr _servAddr;

        int _sockfd;
        std::unordered_map<int ,int> _port_to_sockfd;
        std::set<int> _listen_fds;
        epoll_event  _MessageQueue[1024];//fd queue
        
        //httpRequest _httpRequest;
        //HTTPResponse _httpResponse;
        
        //std::mutex _server_mutex;

        //std::conditional_variable  _have_events;

        //const _maxQueueSize;


//===============================================================;     int initSocket();
};