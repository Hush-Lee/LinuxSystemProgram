#ifndef Channel_HPP
#define Channel_HPP
#include "Epoll.hpp"
#include <cstdint>
class Channel{
    int fd;
    Epoll* ep;
    uint32_t events;
    uint32_t revetns;

    bool inPoll;

public:
    Channel(int fd);

};
#endif