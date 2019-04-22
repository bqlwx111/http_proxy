#include "httpserver.h"
#include <string>
#include <memory>
#include <condition_variable>
#include <unordered_map>

typedef std::shared_ptr<httpServer> httpServerPtr;

class app public:httpServer
{
    public:
        app();
        ~app();

        virtual void run();
        int addServer(std::string& api,httpServerPtr server);
        int rmServer(std::string& api);

        //GET, PUT, HEAD, POST, NOT_IMPLEMENTED
        virtual httpServerReturnType get();
        virtual httpServerReturnType put();
        virtual httpServerReturnType head();
        virtual httpServerReturnType post();
        virtual httpServerReturnType not_implemented();

    private:

        std::unordered_map<std::string,httpServerPtr> servers;
        //std::unordered_map<std::string,std::string>  configure
}