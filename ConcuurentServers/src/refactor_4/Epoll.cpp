#include "Epoll.hpp"
#include <cstdint>
#include <sys/epoll.h>
#include "util.hpp"
#include <strings.h>
#include <unistd.h>
#include "Channel.hpp"
const int MAX_EVENTS=1000;
Epoll::Epoll():ep_fd(-1),events(nullptr){
    ep_fd=epoll_create1(0);
    errif(ep_fd==-1,"error creat epoll\n");
    events=new epoll_event[MAX_EVENTS];
    bzero(events,sizeof(*events)*MAX_EVENTS);
}

Epoll::~Epoll(){
    if(ep_fd!=-1){
        close(ep_fd);
        ep_fd=-1;
    }
    delete []events;
}

void Epoll::add_fd(int fd,uint32_t op){
    epoll_event ev;
    ev.data.fd=fd;
    ev.events=op;

    errif(epoll_ctl(ep_fd,EPOLL_CTL_ADD,fd,&ev)==-1,"error add fd to epoll");
}


void Epoll::update_channel(Channel* ch){
        
}
