#include "Socket.hpp"
#include "InetAddress.hpp"
#include "util.hpp"
#include <sys/socket.h>
#include <fcntl.h>
Socket::Socket():fd(-1) {
    fd=socket(AF_INET, SOCK_STREAM, 0);
    errif(fd==-1, "socket creat failed\n");
}
Socket::Socket(int fd):fd(fd) {
    errif(fd==-1, "socket creat failed\n");
}
void Socket::bind(InetAddress*add) {
    errif(::bind(fd,(struct sockaddr*)&add->addr, add->addrlen)==-1,"socket bind failed\n");
}

void Socket::listen() {
    errif((::listen(this->fd,SOMAXCONN)==-1),"socket listen failed\n");
}

int  Socket::accept(InetAddress*addr) { 
    int clnt_fd = ::accept(this->fd,(struct sockaddr*)&addr->addr, &addr->addrlen);
    errif(clnt_fd==-1, "socket accept failed\n");
    return clnt_fd;
    
}

void Socket::setnonblocking() {
    fcntl(fd,F_SETFD,fcntl(fd,F_GETFD,0)|O_NONBLOCK);
}

void Socket::connect(InetAddress* addr){
    int serv=::connect(fd, (struct sockaddr*)&addr->addr, addr->addrlen);
    errif(serv==-1, "connect failed\n");
}