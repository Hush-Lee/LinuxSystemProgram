#ifndef UTIL_H
#define UTIL_H
#include <netinet/in.h>
namespace hush{
    void errif(bool condition,const char* message);
    int Socket(int family,int type,int protocol);
    int Bind(int sockfd,const struct sockaddr* addr,socklen_t addrlen);
    int Listen(int sockfd,int backlog);
    int Accept(int sockfd,struct sockaddr* addr,socklen_t* addrlen);
    int Connect(int sockfd,const struct sockaddr* addr,socklen_t addrlen);
}
#endif