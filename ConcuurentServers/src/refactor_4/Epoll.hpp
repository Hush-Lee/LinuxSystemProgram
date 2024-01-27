#ifndef EPOLL_HPP
#define EPOLL_HPP
#include <sys/epoll.h>
#include <vector>
class Channel;
class Epoll{
    int ep_fd;
    struct epoll_event *events;
public:
    Epoll();
    ~Epoll();
    void add_fd(int fd,uint32_t op);
    void update_channel(Channel*channel);
    std::vector<epoll_event*> poll(int timeout=-1);

};

#endif