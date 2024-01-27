#include "Channel.hpp"
#include <cstdint>
#include <sys/epoll.h>
#include "EventLoop.hpp"

Channel::Channel(EventLoop *loop,int fd):loop(loop),ep_fd(fd),events(0),revents(0){

}


int Channel::getFd(){
    return ep_fd;
}
uint32_t Channel::getEvents(){
    return events;
}

uint32_t Channel::getRevents(){
    return revents;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setRevents(uint32_t revents){
    this->revents = revents;
}
void Channel::handleEvent(){
    callback();
}

void Channel::enableReading(){
    events = EPOLLIN & EPOLLET;
    loop->updateChannle(this);
}

void Channel::setCallback(std::function<void()>cb){
    callback = cb;
}

void Channel::setInEpoll(){
    inEpoll=true;
}


