#ifndef SOCKET_HPP
#define SOCKET_HPP
#include "common.hpp"
#include <cstdint>
#include <string_view>
#include <unistd.h>
class Socket{
    DISALLOW_COPY_AND_MOVE(Socket);
    int _fd;
public:
    Socket();
    ~Socket();
    void set_fd(int fd);
    int fd();
    std::string_view get_addr();
    RC Creat();
    RC Bind(const char * IP,uint16_t port)const;
    RC Listen()const;
    RC Accept(int &clnt_fd)const;
    RC Connect(const char* IP,uint16_t port)const;
    RC setNonblocking()const;
    bool IsNonblocking()const;
    size_t RecvBufSize()const;
};

#endif