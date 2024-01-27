#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "InetAddress.hpp"
#include "util.hpp"
class Socket{
    int fd;
public:
    Socket(int sock_fd);
    Socket();
    void listen();
    void bind(InetAddress* addr);
    int  accept(InetAddress* addr);
    void connect(InetAddress* addr);
    void setnonblocking();
    int get_fd(){return fd;}
};
#endif