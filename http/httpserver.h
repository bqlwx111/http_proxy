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
        int run ();

        void setUrl(std::string& );
        void setPort(int);
        void setIp(std::string& );

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
        httpRequest _httpRequest;
        HTTPResponse _httpResponse;
        
        std::unoredered_map<int ,int > _port_to_sockfd;

        epoll_event  _MessageQueue[1024];//fd queue
        //std::mutex _server_mutex;

        //std::conditional_variable  _have_events;


        //const _maxQueueSize;


//===============================================================
        int initSocket();
        

}