#include <string>
#include "httpRequest.h"
#include "httpResponse.h"
#include <conditional_variable>
#include <mutex>
#include <lock_guard>
#include <queue>
struct  httpServerReturnType
{
    
};

class httpServer
{
    public:
        httpServer();
        ~httpServer();
//==============================================
        int run ();

        void setUrl(std::string& );
        void setPort(int);
        void setIp(std::string& );
        void setConVar(bool);

        //GET, PUT, HEAD, POST, NOT_IMPLEMENTED
        virtual httpServerReturnType get();
        virtual httpServerReturnType put();
        virtual httpServerReturnType head();
        virtual httpServerReturnType post();
        virtual httpServerReturnType not_implemented();
//===========================================================

        int handleRequest();

    private:
        std::string _ip;
        std::string _url;
        int _port;

        int _sockfd;
        httpRequest _httpRequest;
        HTTPResponse _httpResponse;

        std::conditional_variable  _have_events;

        std::queue<int>  _MessageQueue;//fd queue

        const _maxQueueSize;

        std::mutex _server_mutex;

//===============================================================
        

}