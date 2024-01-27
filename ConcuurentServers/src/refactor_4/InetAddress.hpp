#ifndef INETADDRESS_HPP
#define INETADDRESS_HPP
#include <arpa/inet.h>

class InetAddress{
public:
    struct sockaddr_in addr;
    socklen_t addrlen;
    InetAddress();
    InetAddress(const char* host, const char* port);
    ~InetAddress();
};


#endif