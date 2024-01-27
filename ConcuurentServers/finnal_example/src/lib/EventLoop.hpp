#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP
#include "common.hpp"
#include <memory>

class EventLoop {
  std::unique_ptr<Poller> poller_;

public:
  DISALLOW_COPY_AND_MOVE(EventLoop);
  EventLoop();
  ~EventLoop();
  void Loop() const;
  void UpdateChannel(Channel *ch) const;
  void DeleteChannel(Channel *ch) const;
};


#endif