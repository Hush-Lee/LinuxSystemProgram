#include "util.h"
#include <stdio.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
const int BUFFER_SIZE = 1024;
int main(int argc, char *argv[]) {
    if(argc!= 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    int sock_fd = hush::Socket(AF_INET, SOCK_STREAM,0);
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080);
    hush::Connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    while(true)   {
        char buf[BUFFER_SIZE];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        size_t write_len=write(sock_fd, buf, sizeof(buf));
        if(write_len < 0) {
            printf("connection was already closed,can't write any more\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        size_t read_len=read(sock_fd,buf, sizeof(buf));
        if(read_len > 0){
            printf("message from server :%s\n",buf);
        }else if(read_len == 0){
            printf("server closed the connection\n");
            break;
        }else if(read_len==-1){
            close(sock_fd);
            hush::errif(true, "connection error");
        }
    }
    close(sock_fd);
    return 0;
}