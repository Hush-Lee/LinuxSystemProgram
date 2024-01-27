#include "Socket.hpp"
#include <fcntl.h>
#include <sys/socket.h>
#include "InetAddress.hpp"
#include "util.hpp"
Socket::Socket():sock_fd(-1){
    sock_fd=socket(AF_INET,SOCK_STREAM,0);
    errif(sock_fd==-1,"socket creat error\n");
}

Socket::Socket(int fd):sock_fd(fd){
    errif(sock_fd==-1,"socket copy error\n");
}

void Socket::Listen(){
    errif(::listen(sock_fd,5)==-1,"Listen error");
}

void Socket::Bind(InetAddress* ia){
    errif(::bind(sock_fd, (struct sockaddr*)&ia->addr,ia->addrlen)==-1,"bind error");
}

void Socket::Accept(InetAddress* ia){
    errif(::accept(sock_fd,(struct sockaddr*)&ia->addr, &ia->addrlen),"accept error");
}

void Socket::SetNonblocking(){
    fcntl(sock_fd,F_SETFL,fcntl(sock_fd,F_GETFL)& O_NONBLOCK);
}
int Socket::get_fd(){
    return sock_fd;
}