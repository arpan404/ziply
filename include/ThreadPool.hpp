#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool
{
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;

public:
    explicit ThreadPool(size_t threads);
    template <class F>
    void enqueue(F &&task);
    ~ThreadPool();
};

#endif
