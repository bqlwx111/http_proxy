#include <string>
#include "httpRequest.h"
#include "httpResponse.h"
#include <conditional_variable>
#include <mutex>
#include <queue>
#include <unordered_map>
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

        void setUrl(std::string& url){_url=url;};
        void setPort(int port){_port=port;};
        void setIp(std::string& ip){_ip=ip;};

        int getEpollfd(){return _epoll_fd;}
        epoll_event* getMessageQueue(){return _MessageQueue};

        void addsocket();

        //GET, PUT, HEAD, POST, NOT_IMPLEMENTED
        virtual httpServerReturnType get();
        virtual httpServerReturnType put();
        virtual httpServerReturnType head();
        virtual httpServerReturnType post();
        virtual httpServerReturnType not_implemented();
//===========================================================q

        int handleRequest();

    private:
        std::string _ip;
        std::string _url;
        int _port;
        int _epoll_fd;

        sockaddr_in _servAddr;

        int _sockfd;
        std::unoredered_map<int ,int > _port_to_sockfd;
        epoll_event  _MessageQueue[1024];//fd queue
        //httpRequest _httpRequest;
        //HTTPResponse _httpResponse;
        

        //std::mutex _server_mutex;

        //std::conditional_variable  _have_events;


        //const _maxQueueSize;


//===============================================================
        int initSocket();
        

}