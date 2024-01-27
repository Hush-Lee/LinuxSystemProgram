#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP
#include "Epoll.hpp"
#include "Channel.hpp"

class EventLoop{
    Epoll * ep;
    bool  quit;
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void updateChannle(Channel* channel);
};
#endif