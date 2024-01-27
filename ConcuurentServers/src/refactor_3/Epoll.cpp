#include "Epoll.hpp"
#include "Channel.hpp"
#include "util.hpp"
#include <strings.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
Epoll::Epoll():ep_fd(-1),events(nullptr) {
    ep_fd=epoll_create1(0);
    errif(ep_fd==-1, "epoll_create1 failed\n");
    events=new epoll_event[MAX_EVENT_SIZE];
    bzero(events, MAX_EVENT_SIZE*sizeof(epoll_event));
}
Epoll::~Epoll(){
    if(ep_fd!=-1){
        close(ep_fd);
        ep_fd=-1;
    }
    delete [] events;
}


void Epoll::updateChannle(Channel *channel){
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr=channel;
    ev.events=channel->getEvents();
    if(!channel->getInEpoll()){
        channel->setInEpoll();
        errif(epoll_ctl(ep_fd, EPOLL_CTL_ADD, channel->getFd(), &ev)==-1,"Failed to add event");
    }else{
        errif(epoll_ctl(ep_fd, EPOLL_CTL_MOD, channel->getFd(), &ev)==-1,"Failed to modify event");
    }
}

std::vector<Channel*> Epoll::poll(int timeout){
    int nfds=epoll_wait(ep_fd, events, MAX_EVENT_SIZE, timeout);
    errif(nfds==-1, "epoll_wait failed\n");
    std::vector<Channel*> active_channels;
    for(int i=0;i<nfds;i++){
        Channel*channel=(Channel*)events[i].data.ptr;
        channel->setRevents(events[i].events);
        active_channels.push_back(channel);
    }
    return active_channels;
}
