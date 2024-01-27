#include "lib/Poller.hpp"
#include <sys/epoll.h>
#include "lib/common.hpp"
#include "lib/util.hpp"
#include <cstring>
#include <unistd.h>
#include <vector>
#include "lib/Channel.hpp"
const int MAX_EVENTS = 1000;
//#ifdef OS_LINUX
Poller::Poller() {
  fd_ = epoll_create1(0);
  ErrorIf(fd_ == -1, "epoll creat error!/n");
  events_ = new epoll_event[MAX_EVENTS];
  memset(events_,0,sizeof(*events_)*MAX_EVENTS);
}

Poller::~Poller() {
  if (fd_ != -1) {
    close(fd_);
  }
  delete[] events_;
}

std::vector<Channel *> Poller::Poll(long timeout) const {
  std::vector<Channel *> active_channels;
  int nfds = epoll_wait(fd_, events_, MAX_EVENTS, timeout);
  ErrorIf(nfds == -1, "epoll_wait error!\n");
  for (int i = 0; i < nfds; ++i) {
    Channel *ch = (Channel *)events_[i].data.ptr;
    int events = events_[i].events;
    if (events & EPOLLIN) {
      ch->set_ready_event(Channel::READ_EVENT);
    }
    if (events & EPOLLOUT) {
      ch->set_ready_event(Channel::WRITE_EVENT);
    }
    if (events & EPOLLET) {
      ch->set_ready_event(Channel::ET);
    }
    active_channels.push_back(ch);
  }
  return active_channels;
}


void Poller::UpdateChannel(Channel *ch) {
  int sockfd = ch->fd();
  struct epoll_event ev {};
  ev.data.ptr = ch;
  if (ch->listen_events() & Channel::READ_EVENT) {
    ev.events |= EPOLLIN|EPOLLPRI;
  }
  if (ch->listen_events() & Channel::WRITE_EVENT) {
    ev.events|=EPOLLOUT;
  }
  if (ch->listen_events() & Channel::ET) {
    ev.events|=EPOLLET;
  }
  if (!ch->exits()) {
    ErrorIf(epoll_ctl(fd_, EPOLL_CTL_ADD, sockfd, &ev)==-1,"epoll add error\n" );
  }else {
    ErrorIf(epoll_ctl(fd_, EPOLL_CTL_MOD, sockfd, &ev)==-1,"epoll modify error\n");
  }
}

void Poller::DeleteChannel(Channel *ch) {
  int sockfd = ch->fd();
  ErrorIf(epoll_ctl(fd_, EPOLL_CTL_DEL, sockfd, nullptr) == -1,
          "epoll delete error\n");
  ch->set_exist(false);
}


//#endif