#include "lib/Channel.hpp"
#include "lib/common.hpp"
#include "lib/EventLoop.hpp"
#include <utility>

const short Channel::READ_EVENT = 1;
const short Channel::WRITE_EVENT = 2;
const short Channel::ET = 4;
Channel::Channel(int fd, EventLoop *loop)
    : fd_(fd), loop_(loop), listen_events_(0), ready_events_(0), exist_(false) {

}
Channel::~Channel() {
  loop_->DeleteChannel(this);
  }
void Channel::HandleEvent() const {
  if (READ_EVENT & ready_events_) {
    read_callback();
  }
  if (WRITE_EVENT & ready_events_) {
    write_callback();
  }
}


void Channel::EnableWrite() {
  listen_events_ |= WRITE_EVENT;
  loop_->UpdateChannel(this);
}

void Channel::EnableRead() {
  listen_events_ |= READ_EVENT;
  loop_->UpdateChannel(this);
}

void Channel::EnableET() {
  listen_events_ |= ET;
  loop_->UpdateChannel(this);
}

int Channel::fd() const { return fd_; }

short Channel::listen_events() const { return listen_events_; }

short Channel::ready_events() const { return ready_events_; }

bool Channel::exits() const { return exist_; }

void Channel::set_exist(bool in) { exist_ = in; }

void Channel::set_ready_event(short ev) {
  if (ev & READ_EVENT) {
    ready_events_|=READ_EVENT;
  }
  if (ev & WRITE_EVENT) {
    ready_events_|=WRITE_EVENT;
  }
  if (ev & ET) {
    ready_events_|=ET;
  }
}

void Channel::set_read_callback(std::function<void()> const &callback) {
  read_callback=std::move(callback);
}

void Channel::set_write_callback(std::function<void()> const &callback) {
  write_callback=std::move(callback);
}
