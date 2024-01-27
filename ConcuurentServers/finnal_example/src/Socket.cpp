#include"lib/Socket.hpp"
#include "lib/common.hpp"
#include <arpa/inet.h>
#include <cassert>


#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string_view>
#include <cstring>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
Socket::Socket():_fd(-1){

}

Socket::~Socket(){
    if(_fd!=-1){
        close(_fd);
        _fd=-1;
    }
}

void Socket::set_fd(int fd){
    _fd=fd;
}

std::string_view Socket::get_addr(){
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    socklen_t len=sizeof(addr);
    if(getpeername(_fd, (struct sockaddr*)&addr,&len)==-1){
        return "";
    }
    std::string ret(inet_ntoa(addr.sin_addr));
    ret+=":";
    ret+=std::to_string(htons(addr.sin_port));
    return ret;
}

RC Socket::setNonblocking()const{
    if(fcntl(_fd, F_SETFL,fcntl(_fd,F_GETFL)|O_NONBLOCK)==-1){
        perror("Socket set nonblocking failed");
        return RC::RC_SOCKET_ERROR;
    }
    return RC_SUCCESS;
}

RC Socket::Bind(const char* ip,uint16_t port)const{
    struct sockaddr_in addr;
    memset(&addr,0, sizeof(addr));
    addr.sin_addr.s_addr=inet_addr(ip);
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    if(::bind(_fd,(struct sockaddr*)&addr,sizeof(addr))==-1){
        perror("Failed to bind socket!");
        return RC::RC_SOCKET_ERROR;
    }
    return RC_SUCCESS;
}

RC Socket::Creat(){
    assert(_fd!=-1);
    _fd=::socket(AF_INET,SOCK_STREAM,0);
    if(_fd==-1){
        perror("Failed to creat socket\n");
        return RC::RC_SOCKET_ERROR;
    }
    return RC::RC_SUCCESS;
}

RC Socket::Accept(int & clnt_fd)const{
    assert(_fd!=-1);
    if((clnt_fd=::accept(_fd, nullptr,nullptr))==-1){
        perror("Socket bind error\n");
        return RC::RC_SOCKET_ERROR;
    }
    return RC::RC_SUCCESS;
}
size_t Socket::RecvBufSize()const{
    size_t size=-1;
    if(ioctl(_fd,FIONREAD,&size)==-1){
        perror("Socket read error\n");
    }
    return size;
}
bool Socket::IsNonblocking()const{
    return (fcntl(_fd,F_GETFL)|O_NONBLOCK)!=0;
}

RC Socket::Listen()const{
    assert(_fd);
    if(::listen(_fd,5)==-1){
        perror("Socket listen error \n");
        return RC::RC_SOCKET_ERROR;
    }
    return RC::RC_SUCCESS;
}

RC Socket::Connect(const char* IP,uint16_t port)const{
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_addr.s_addr=inet_addr(IP);
    addr.sin_port=htons(port);
    addr.sin_family=AF_INET;
    socklen_t len=sizeof(addr);
    if(::connect(_fd, (struct sockaddr*)&addr,sizeof(addr))==-1){
        perror("Socket Connection Error\n");
        return RC::RC_SOCKET_ERROR;
    }
    return RC::RC_SUCCESS;
}