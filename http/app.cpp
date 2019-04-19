#include "app.h"
#include<string>
#include<memory>
#include<sys/epoll.h>
#include<sys/socket.h>
app::app()
{
    _epoll_size=1024;
    _epoll_fd=0;
    _have_event=false;

    _epoll_fd=epoll_create(_epoll_size);

}
void app::run()
{
    for(auto& i:servers)
        i->run();
    
}
//=========================================================
typedef std::shared_ptr<httpServer> httpServerPtr;
int app::addServer(std::string& api,httpServerPtr server)
{
    servers[api]=std::move(server);
    return 1;
}
int app::rmServer(std::string& api)
{
    auto i =servers.find(api);
    if(i!=servers.end())
        servers.erase(i);
    return 1;
}
//===========================
int app::epollRegist(int fd)
{

    epoll_ct(_epoll_fd,EPOLL_CTL_ADD,fd,);
}
int aPP::epollUnRegist(int fd)
{

}
