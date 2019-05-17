#include "app.h"
#include<string>
#include<memory>
#include<sys/epoll.h>
#include<sys/socket.h>
app::app()
{
    _epoll_fd=epoll_create(_epoll_size);

}
void app::run()
{
    for(auto& i:servers)
        i->run();

    app.setPort(7000);
    app.setIp("0.0.0.0");
    app.addsocket();

    while(true)
    {
        int events_num=epoll_wait(getEpollfd(),getMessageQueue(),1024,-1);
        for(int i=0;i<events_num;i++)
        {
            thread t(handleRequest);
            t.detach();
        }

    }
    
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