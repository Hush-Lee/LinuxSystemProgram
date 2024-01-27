#ifndef THREADPOLL_HPP
#define THREADPOLL_HPP

#include "common.hpp"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
class ThreadPool{
public:
    DISALLOW_COPY_AND_MOVE(ThreadPool);
    explicit ThreadPool(unsigned int size=std::thread::hardware_concurrency());
    ~ThreadPool();
    template<class F,class... Args>
    auto Add(F &&f,Args&&... args)->std::future<typename std::invoke_result<F,Args...>::type>;
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_variable_;
    std::atomic<bool>stop_{false};
};

template <class F,class... Args>
auto ThreadPool::Add(F&&f,Args&&... args)->std::future<typename std::invoke_result<F,Args...>::type>{
    using return_type = typename std::invoke_result<F, Args...>::type;
    auto task=std::make_shared<std::packaged_task<return_type>>(std::bind(std::forward<F>(f),std::forward<Args>(args)...));
    std::future<return_type>res=task->get_future();
    {
        std::unique_lock<std::mutex>lock(queue_mutex_);
        if(stop_){
            throw std::runtime_error("enqueue on stopped TheadPool");
        }
        tasks_.emplace([task](){(*task)();});
    }
    condition_variable_.notify_one();
    return res;
}


#endif