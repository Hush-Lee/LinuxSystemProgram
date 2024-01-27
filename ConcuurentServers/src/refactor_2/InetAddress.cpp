#include "InetAddress.hpp"
#include <cstring>
InetAddress::InetAddress(const char *ip,uint16_t port) {
    memset(&this->addr, 0, sizeof(this->addr));
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(port);
    this->addr.sin_addr.s_addr = inet_addr(ip);
    this->addrlen = sizeof(this->addr);
}
InetAddress::InetAddress():addrlen(sizeof(this->addr)){
    memset(&this->addr, 0, sizeof(this->addr));
    
}
InetAddress::~InetAddress(){

}