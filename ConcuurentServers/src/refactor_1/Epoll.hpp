#ifndef EPOLL_HPP
#define EPOLL_HPP
#include <vector>
#include "Channel.hpp"
#include "util.hpp"
#include <sys/epoll.h>
class Epoll{
    int ep_fd;
    struct epoll_event *events;
public:
    Epoll();
    ~Epoll();
    void add(int fd,uint32_t op);
    std::vector<epoll_event> poll(int timeout=-1);
    //void updateChannel(Channel*channel);
};
#endif
