#include "lib/EventLoop.hpp"
#include <memory>
#include "lib/Poller.hpp"
#include "lib/Channel.hpp"
#include "lib/common.hpp"
EventLoop::EventLoop() { poller_ = std::make_unique<Poller>(); }

EventLoop::~EventLoop() {}

void EventLoop::Loop() const {
  while (true) {
    for (Channel *achive_ch : poller_->Poll()) {
      achive_ch->HandleEvent();
    }
  }
}
void EventLoop::DeleteChannel(Channel *ch) const { poller_->DeleteChannel(ch); }

void EventLoop::UpdateChannel(Channel *ch) const { poller_->UpdateChannel(ch); }
