#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include "common.hpp"
#include <functional>
#include <memory>
#include <unordered_map>
class TcpServer {
public:
  DISALLOW_COPY_AND_MOVE(TcpServer);
  TcpServer();
  ~TcpServer();
  void start();

  RC NewConnection(int fd);
  RC DeleteConnection(int fd);

  void onConnect(std::function<void(Connection *)> fn);
  void onRecv(std::function<void(Connection *)> fn);

private:
  std::unique_ptr<EventLoop> main_reactor_;//事务处理
  std::unique_ptr<Acceptor> acceptor_;//接受连接

  std::unordered_map<int, std::unique_ptr<Connection>> connections_;//存放连接，那线程池是干嘛的？
  std::vector<std::unique_ptr<EventLoop>> sub_reactors_;

  std::unique_ptr<ThreadPool> thread_pool_;

  std::function<void(Connection *)> on_connect_;
  std::function<void(Connection *)> on_recv_;
};
#endif