
#include "lib/TcpServer.hpp"
#include "lib/EventLoop.hpp"
#include "lib/Acceptor.hpp"
#include "lib/common.hpp"
#include "lib/ThreadPool.hpp"
#include "lib/Connection.hpp"
#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <thread>
#include <utility>
TcpServer::TcpServer() {
  main_reactor_ = std::make_unique<EventLoop>();
  acceptor_ = std::make_unique<Acceptor>(main_reactor_.get());
  std::function<void(int)> cb =
      std::bind(&TcpServer::NewConnection, this, std::placeholders::_1);
  acceptor_->set_new_connection_callback(cb);

  unsigned int size = std::thread::hardware_concurrency();
  thread_pool_ = std::make_unique<ThreadPool>(size);

  for (size_t i = 0; i < size; ++i) {
    std::unique_ptr<EventLoop> sub_reactor = std::make_unique<EventLoop>();
    sub_reactors_.push_back(sub_reactor);
  }
}

TcpServer::~TcpServer() {}

void TcpServer::start() {
  for (size_t i = 0; i < sub_reactors_.size(); ++i) {
    std::function<void()> sub_loop =
        std::bind(&EventLoop::Loop, sub_reactors_[i].get());
    thread_pool_->Add(std::move(sub_loop));
  }
  main_reactor_->Loop();
}

RC TcpServer::NewConnection(int fd) {
  assert(fd != -1);
  uint64_t random = fd % sub_reactors_.size();
  std::unique_ptr<Connection> conn =
      std::make_unique<Connection>(fd, sub_reactors_[random].get());
  std::function<void(int)> cb =
      std::bind(&TcpServer::DeleteConnection, this, std::placeholders::_1);
  conn->set_delete_connection(cb);
  conn->set_on_recv(on_recv_);

  connections_[fd] = std::move(conn);

  if (on_connect_) {
    on_connect_(connections_[fd].get());
  }
  return RC::RC_SUCCESS;
}

RC TcpServer::DeleteConnection(int fd) {
  auto it = connections_.find(fd);
  assert(it != connections_.end());
  connections_.erase(fd);
  return RC::RC_SUCCESS;
}

void TcpServer::onConnect(std::function<void(Connection *)> fn) {
  on_connect_ = std::move(fn);
}

void TcpServer::onRecv(std::function<void(Connection *)> fn) {
  on_recv_ = std::move(fn);
}
