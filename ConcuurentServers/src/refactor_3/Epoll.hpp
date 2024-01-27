#include <sys/epoll.h>
#include <vector>
#include "Channel.hpp"
#include "util.hpp"
class Epoll{
    int  ep_fd;
    struct epoll_event *events;

public:
    Epoll();
    ~Epoll();
    void addFd(int fd, uint32_t op);

    std::vector<Channel*>poll(int timeout=-1);
    void updateChannle(Channel*channel);
};