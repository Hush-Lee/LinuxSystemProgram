#ifndef POLLER_HPP
#define POLLER_HPP
#include <vector>
#include "common.hpp"

#ifdef OS_LINUIX
#include <sys/epoll.h>
#endif
#ifdef OS_MACOS
#include <sys/event.h>
#endif

class Poller {
public:
  DISALLOW_COPY_AND_MOVE(Poller);
  Poller();
  ~Poller();

  std::vector<Channel *> Poll(long timeout=-1)const;
  void UpdateChannel(Channel *ch) ;
  void DeleteChannel(Channel *ch) ;

private:
  int fd_;
  struct epoll_event *events_{nullptr};

#ifdef OS_MACOS
  struct kevent *events_{nullptr};
#endif
};

#endif