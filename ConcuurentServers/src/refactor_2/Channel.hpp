#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <sys/epoll.h>
#include <sys/types.h>
class Epoll;
class Channel{
    Epoll * ep;
    int fd;
    u_int32_t events;
    u_int32_t revents;
    bool inPoll;
public:
    Channel(Epoll * ep, int fd);
    ~Channel();
    void enableReading();
    int get_fd() ;
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInPoll();
    void setInPoll();
    void setRevents(u_int32_t revents);

};



#endif