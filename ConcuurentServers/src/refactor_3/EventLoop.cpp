#include "EventLoop.hpp"
#include "Channel.hpp"
#include <sys/epoll.h>
#include <vector>

EventLoop::EventLoop():ep(nullptr),quit(false){
    ep=new Epoll();
}
EventLoop::~EventLoop(){
    delete ep;
}
void EventLoop::loop(){
    while(!quit){
        std::vector<Channel*> chs;
        chs=ep->poll();
        for(auto ch:chs){
            ch->handleEvent();
        }
    }
}
void EventLoop::updateChannle(Channel* ch){
    ep->updateChannle(ch);
}