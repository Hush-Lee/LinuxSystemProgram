#include "InetAddress.hpp"
#include "Socket.hpp"
#include <cstddef>
#include <cstring>
#include <linux/close_range.h>
#include <unistd.h>
const int BUF_SIZE = 1024;
int main(int argc, char **argv){
    if(argc < 3){
        printf( "%s Usage: <IP> <port>", argv[0]);
        return 1;
    }
    Socket* sock=new Socket();
    InetAddress* addr=new InetAddress(argv[1],atoi(argv[2]));
    sock->connect(addr);
    while(true){
        char buf[BUF_SIZE];
        memset(buf, 0, sizeof(buf));
        scanf("%s", buf);
        size_t write_size=write(sock->get_fd(), buf, sizeof(buf));
        if(write_size==-1){
            printf("Connection has already closed\n");
            break;
        }
        memset(buf,0,sizeof(buf));
        size_t read_size=read(sock->get_fd(), buf, sizeof(buf));
        if(read_size>0){
            printf("message from server: %s\n", buf);
        }else if(read_size==0){
            printf("Connection has already closed\n");
            break;
        }else if(read_size==-1){
            close(sock->get_fd());
            errif(true,"Read failed\n");
        }

    }
    close(sock->get_fd());
    return 0;
} 