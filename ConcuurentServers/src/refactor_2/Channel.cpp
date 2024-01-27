#include "Channel.hpp"
#include "Epoll.hpp"
#include <cstdint>
#include <sys/epoll.h>


Channel::Channel(Epoll *epoll, int fd) : ep(nullptr),fd(fd),events(0),revents(0),inPoll(false){

}
Channel::~Channel(){

}
void Channel::enableReading(){
    events = EPOLLIN|EPOLLET;
    ep->updateChannel(this);
}
int Channel::get_fd() {
    return fd;
}

uint32_t Channel::getEvents() {
    return events;
}

uint32_t Channel::getRevents() {
    return revents;
}

void Channel::setRevents(uint32_t revents) {
    this->revents = revents;
}

bool Channel::getInPoll() {
    return inPoll;
}

void Channel::setInPoll() {
    this->inPoll = true;
}
