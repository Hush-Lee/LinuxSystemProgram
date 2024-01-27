#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <cstdint>
#include <functional>
#include <sys/epoll.h>

class EventLoop;
const int MAX_EVENT_SIZE=1000;
class Channel{
    int ep_fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    EventLoop* loop;
    std::function<void()>callback;
public:
    Channel(EventLoop* loop,int fd);
    ~Channel();

    void handleEvent();

    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();
    void setRevents(uint32_t revents);
    void setCallback(std::function<void()> callback);


};
#endif