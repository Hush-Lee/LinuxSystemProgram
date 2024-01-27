#include "InetAddress.hpp"
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
InetAddress::InetAddress(){
    bzero(&addr, sizeof(addr));
}

InetAddress::InetAddress(const char* host, const char*port){
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htonl(atoi(port));
    addrlen = sizeof(addr);
}

InetAddress::~InetAddress(){
    
}