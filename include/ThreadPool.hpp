#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
private:
  std::vector<std::thread> workers;
  std::queue<std::function<std::future<void>()>> tasks;
  std::mutex queueMutex;
  std::condition_variable condition;
  bool stop = false;

public:
  explicit ThreadPool(size_t threads);
  template <class F> void enqueue(F &&task);
  void wait();
};

template <class F> void ThreadPool::enqueue(F &&task) {
  {
    std::unique_lock<std::mutex> lock(queueMutex);
    tasks.emplace(std::forward<F>(task));
  }

  condition.notify_one();
}

#endif
