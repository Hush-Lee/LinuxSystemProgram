#ifndef SERVER_HPP
#define SERVER_HPP
#include "Channel.hpp"
class EventLoop;
class Socket;
class Server {
    EventLoop *loop;

public:
    Server(EventLoop *loop);
    ~Server();
    void newConnection(Socket*serv_socket);
    void handleReadEvent(int);

};

#endif