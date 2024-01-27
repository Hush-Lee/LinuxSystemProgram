#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
void hush::errif(bool condition, const char *message){
    if(condition){
        perror(message);
        exit(EXIT_FAILURE);
    }
}
int hush::Socket(int family, int type, int protocol){
    int n = socket(family,type,protocol);
    if(n<0){
        perror("socket error");
        exit(1);
    }
    return n;
}

int hush::Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
    int n;
    if((n=connect(sockfd, addr, addrlen))<0){
        perror("connect error");
        exit(1);
    }
    return n;
    
}

int hush::Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
    int n;
    if((n=bind(sockfd, addr, addrlen))<0){
        perror("bind error");
        exit(1);
    }
    return n;
}

int hush::Listen(int sockfd, int backlog){
    int n=listen(sockfd, backlog);
    if(n<0){
        perror("listen error");
        exit(1);
    }
    return n;
}
int hush::Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen){
    int n=accept(sockfd, addr, addrlen);
    if(n<0){
        perror("accept error");
        exit(1);
    }
    return n;
}