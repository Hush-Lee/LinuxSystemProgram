#include "Channel.hpp"
#include "Epoll.hpp"
#include <sys/epoll.h>

Channel::Channel(Epoll *epoll, int fd) : ep(nullptr),fd(fd),events(0),revents(0),inPoll(false){

}
Channel::~Channel(){

}
void Channel::enableReading(){
    events = EPOLLIN|EPOLLET;
    //ep->updateChannel(this);
}