#ifndef SOCKET_HPP
#define SOCKET_HPP
#include "InetAddress.hpp"
class Socket{
    int sock_fd;
public:
    Socket();
    Socket(int fd);
    ~Socket();

    void SetNonblocking();
    void Listen();
    void Bind(InetAddress* ia);
    void Accept(InetAddress* ia);
    int get_fd();
};

#endif