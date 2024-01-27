#include "Epoll.hpp"
#include "InetAddress.hpp"
#include "Socket.hpp"
#include <asm-generic/errno-base.h>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <strings.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <error.h>
#include "Channel.hpp"

const int BUFFER_SIZE = 1024;
const int MAX_EVENTS = 1024;
void handleReadEvents(int nevents);
int main(int argc, char *argv[]){
    if(argc<3){
        printf("Usage:%s <ip> <port>\n",argv[0]);
        return -1;
    }
    Socket * serv_socket = new Socket();
    InetAddress * serv_address = new InetAddress(argv[1],atoi(argv[2]));
    serv_socket->bind(serv_address);
    serv_socket->listen();
    Epoll * ep = new Epoll();
    serv_socket->setnonblocking();
    Channel * channel = new Channel(ep,serv_socket->get_fd());
    channel->enableReading();
    while(true){
        std::vector<Channel*> activeChannels=ep->poll();
        for(auto ch: activeChannels){
            if(ch->get_fd()==serv_socket->get_fd()){
                InetAddress *address=new InetAddress();
                Socket *client_socket=new Socket(serv_socket->accept(address));
                printf("new client %d: %s:%d\n",client_socket->get_fd(),inet_ntoa(address->addr.sin_addr),
                ntohs(address->addr.sin_port));
                client_socket->setnonblocking();
                Channel * clnt_channel = new Channel(ep,client_socket->get_fd());
                clnt_channel->enableReading();
            }else if(ch->getRevents()&EPOLLIN){
                handleReadEvents(ch->get_fd());               
            }else{
                printf("deal with somthing else\n");
            }
        }
    }
    delete serv_socket;
    delete serv_address;
}

void handleReadEvents(int event){
    char buf[BUFFER_SIZE];
    while(true){
        memset(buf, 0,sizeof(buf));
        size_t serv_recv = read(event,buf,sizeof(buf));   
        if(serv_recv>0){
            printf("client %d says : %s\n",event,buf);
            write(event,buf,serv_recv);
        }else if(serv_recv==-1&& errno == EINTR){
            printf("client %d continue\n",event);
            continue;
        }else if(serv_recv==-1&&(errno ==EAGAIN||errno ==EWOULDBLOCK)){
            printf("finsh read once,errno:%d\n",errno);
            break;
        }else if(serv_recv==0){
            printf("EOF,client %d  has closed the connection\n",event);
            close(event);
            break;
        }
    }
}