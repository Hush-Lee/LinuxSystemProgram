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
    u_int32_t getEvents();
    u_int32_t getRevents();
    bool getInPoll();
    void setInPoll(bool inPoll);
    void setRevents(u_int32_t revents);

};



#endif