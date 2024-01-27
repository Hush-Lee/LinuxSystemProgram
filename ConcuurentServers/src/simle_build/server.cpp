#include "util.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
const int MAX_EVENTS = 1024;
const int BUFFER_SIZE = 1024;
void setnonblocking(int sockfd){
    fcntl(sockfd,F_SETFD,fcntl(sockfd,F_GETFD,0)|O_NONBLOCK);
}
int main(int argc, char **argv){
    if(argc!= 3){
        printf("Usage: %s <IP> <PORT>\n", argv[0]);
        exit(1);
    }
    int sock_fd=hush::Socket(AF_INET, SOCK_STREAM,0);
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr=inet_addr(argv[1]);
    socklen_t sock_len=sizeof(server_addr);
    hush::Bind(sock_fd, (struct sockaddr*)&server_addr, sock_len);
    hush::Listen(sock_fd, 5);

    int ep_fd=epoll_create(0);
    hush::errif(ep_fd==-1,"epoll_create");
    struct epoll_event events[MAX_EVENTS],ev;
    bzero(&events, sizeof(events));

    bzero(&ev, sizeof(events));
    ev.data.fd=sock_fd;
    ev.events=EPOLLIN|EPOLLET;
    setnonblocking(sock_fd);
    hush::errif(epoll_ctl(ep_fd,EPOLL_CTL_ADD,sock_fd,&ev)==-1,
        "epoll_ctl error");
    while(1){
        int nfds=epoll_wait(ep_fd,events, MAX_EVENTS, -1);
        hush::errif(nfds==-1, "epoll_wait error");
        for(int i=0;i<nfds;i++){
            if(events[i].data.fd==sock_fd){
                struct sockaddr_in client_addr;
                bzero(&client_addr, sizeof(client_addr));
                socklen_t client_len=sizeof(client_addr);
                int client_fd=hush::Accept(sock_fd,
                 (struct sockaddr*)&client_addr, &client_len);
                hush::errif(client_fd==-1, "client_fd error"); 
                
                bzero(&ev, sizeof(events));
                ev.data.fd=client_fd;
                ev.events=EPOLLIN|EPOLLET;
                setnonblocking(client_fd);
                hush::errif(epoll_ctl(ep_fd,EPOLL_CTL_ADD,client_fd,&ev)==-1,
                    "epoll_ctl error");
            }else if(events[i].events & EPOLLIN){
                char buf[BUFFER_SIZE];
                while (true) {
                    bzero(&buf, sizeof(buf));
                    size_t read_size=read(events[i].data.fd, buf, sizeof(buf));
                    if(read_size>0){
                        printf("message from client fd %d:%s", events[i].data.fd,buf);
                        write(events[i].data.fd, buf, sizeof(buf));
                    }else if (read_size==-1&&errno==EINTR){
                        printf("connection continue\n");
                        continue;
                    }else if(read_size==-1&&((errno == EAGAIN) || (errno ==EWOULDBLOCK))){
                        printf("finsh reading once , errno is %d\n",errno);
                        break;
                    }else if(read_size==0){
                        printf("EOF,client fd %d disconnect\n", events[i].data.fd);
                        close(events[i].data.fd);
                        break;
                    }    
                }

            }else{
                printf("something else happened\n");
            }
        }
    }
    close(sock_fd);
    return 0;
    //struct sockaddr_in client_addr;
    //socklen_t client_len=sizeof(client_addr);
    //bzero(&client_addr, sizeof(client_addr));
    //int client_fd=hush::Accept(sock_fd, (struct sockaddr*)&client_addr, &client_len);
   
}