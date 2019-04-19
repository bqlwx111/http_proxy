#include "httpserver.h"
#include <string>
#include <memory>
#include <condition_variable>
#include <unordered_map>

typedef std::shared_ptr<httpServer> httpServerPtr;

class app
{
    public:
        app();
        ~app();

        int addServer(std::string& api,httpServerPtr server);
        int rmServer(std::string& api);

    private:

        std::unordered_map<std::string,httpServerPtr> servers;
        std::unordered_map<std::string,std::string>  configure;


        int _epoll_size;
        int _epoll_fd;
        
        std::condition_variable _have_event;

        int epollRegist();
        int epollUnRegist();




}