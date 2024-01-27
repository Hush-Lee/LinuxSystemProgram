#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include "common.hpp"
#include <memory>
#include <functional>

class Acceptor {
    std::unique_ptr<Socket> socket;
    std::unique_ptr<Channel> channel;
    std::function<void(int)> new_connection_callback;
public:
    DISALLOW_COPY_AND_MOVE(Acceptor);
    explicit Acceptor(EventLoop *loop);
    ~Acceptor();
    RC AcceptConnection()const;
    void set_new_connection_callback(std::function<void(int)>const& callback);

    
};
#endif