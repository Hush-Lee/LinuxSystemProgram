#include "Socket.hpp"
#include "EventLoop.hpp"
#include "Server.hpp"
#include <cstddef>
#include <strings.h>
#include <unistd.h>
const int BUFFER_SIZE = 1024;
Server::Server(EventLoop* loop):loop(loop){
    Socket* serv_sock=new Socket();
    InetAddress *address=new InetAddress("127.0.0.1",8888);
    serv_sock->bind(address);
    serv_sock->listen();
    serv_sock->setnonblocking();

    Channel* serv_channel=new Channel(loop, serv_sock->get_fd());
    std::function<void()> cd = std::bind(&Server::newConnection,this,serv_sock);
    serv_channel->setCallback(cd);
    serv_channel->enableReading();

}

Server::~Server(){
}

void Server::handleReadEvent(int fd){
    char buf[BUFFER_SIZE];
    while(true){
        bzero(buf, sizeof(buf));
        size_t bytes_read=read(fd,buf,BUFFER_SIZE);
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", fd, buf);
            write(fd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", fd);
            close(fd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }

}



void Server::newConnection(Socket* serv_sock){
    InetAddress *clnt_addr=new InetAddress();
    Socket * clnt_sock=new Socket(serv_sock->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->get_fd(),
     inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonblocking();
    Channel* clnt_channel=new Channel(loop,clnt_sock->get_fd());
    std::function<void()> cb = std::bind(&Server::handleReadEvent,this,clnt_sock->get_fd());
    clnt_channel->setCallback(cb);
    clnt_channel->enableReading();

}