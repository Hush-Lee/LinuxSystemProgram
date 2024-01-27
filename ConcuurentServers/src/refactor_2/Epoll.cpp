#include "Epoll.hpp"
#include "Channel.hpp"
#include "util.hpp"
#include <cstdio>
#include <cstring>
#include <strings.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <vector>
const int EPOLL_MAX_EVENTS = 1000;
Epoll::Epoll():ep_fd(-1),events(nullptr) {
    ep_fd=epoll_create1(0);
    errif(ep_fd==-1, "epoll_create1 error\n");
    events=new epoll_event[EPOLL_MAX_EVENTS];
    bzero(events, sizeof(*events)*EPOLL_MAX_EVENTS);
}
Epoll::~Epoll() {
    if(ep_fd!=-1) {
        close(ep_fd);
        ep_fd=-1;
    }
    delete [] events;
}

void Epoll::add(int fd, uint32_t op) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd=fd;
    ev.events=op;
    int ret=epoll_ctl(ep_fd, EPOLL_CTL_ADD, fd, &ev);
    errif(ret==-1, "epoll_ctl error\n");
}

//std::vector<epoll_event> Epoll::poll(int timeout) {
  //  std::vector<epoll_event> active_events;
//    int nfds=epoll_wait(ep_fd, events, EPOLL_MAX_EVENTS, timeout);
//    errif(nfds==-1, "epoll_wait error\n");
//    for(int i=0;i<nfds;i++) {
//        active_events.push_back(events[i]);
//    }
//  return active_events;
//}

std::vector<Channel*>Epoll::poll(int timeout) {
    std::vector<Channel*> activeChannels;
    int nfds = epoll_wait(ep_fd, events, EPOLL_MAX_EVENTS, timeout);
    errif(nfds==-1, "Epoll wait error\n");
    for(int i=0;i<nfds;i++) {
        //printf("%p\n", events[i].data.ptr);
        Channel * ch=(Channel*)events[i].data.ptr;
        ch->setRevents(events[i].events);
        //printf("cast success\n");
        activeChannels.push_back(ch);
    }
    return activeChannels;
}

void Epoll::updateChannel(Channel* channel) {
    int fd=channel->get_fd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events=channel->getEvents();
    if(!channel->getInPoll()){
        errif(epoll_ctl(ep_fd,EPOLL_CTL_ADD,fd,&ev)==-1, "epoll_ctl error\n");
        channel->setInPoll();
    }else{
        errif(epoll_ctl(ep_fd, EPOLL_CTL_MOD,fd, &ev)==-1,"epoll_ctl modify error\n");
    }
}
