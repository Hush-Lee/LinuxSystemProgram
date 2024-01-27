#ifndef INETADDRESS_HPP
#define INETADDRESS_HPP
#include <arpa/inet.h>
class InetAddress{
public:
    struct sockaddr_in addr;
    socklen_t addrlen;
    InetAddress();
    InetAddress(const char *ip,uint16_t port);
    ~InetAddress();
};

#endif