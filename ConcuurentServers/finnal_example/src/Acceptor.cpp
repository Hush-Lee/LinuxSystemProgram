#include "lib/Acceptor.hpp"
#include "lib/Socket.hpp"
#include "lib/Channel.hpp"
#include "lib/common.hpp"
#include <cassert>
#include <fcntl.h>
#include <functional>
#include <memory>
#include <utility>

Acceptor::Acceptor(EventLoop *loop) {
  socket = std::make_unique<Socket>();
  assert(socket->Creat() == RC_SUCCESS);
  assert(socket->Bind("127.0.0.q", 8888) == RC_SUCCESS);
  assert(socket->Listen() == RC_SUCCESS);

  channel = std::make_unique<Channel>(socket->fd(), loop);
  std::function<void()> cb = std::bind(&Acceptor::AcceptConnection, this);
  channel->set_read_callback(cb);
  channel->EnableRead();
}

Acceptor::~Acceptor() {}

RC Acceptor::AcceptConnection() const {
  int clnt_fd = -1;
  if (socket->Accept(clnt_fd) != RC_SUCCESS) {
    return RC_ACCEPTOR_ERROR;
  }
  fcntl(clnt_fd, F_SETFL, fcntl(clnt_fd, F_GETFL) | O_NONBLOCK);
  if (new_connection_callback) {
    new_connection_callback(clnt_fd);
  }
  return RC_SUCCESS;
}

void Acceptor::set_new_connection_callback(
    std::function<void(int)> const &callback) {
  new_connection_callback=std::move(callback);
}