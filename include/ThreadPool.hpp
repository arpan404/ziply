#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

// ThreadPool class manages a pool of worker threads to execute tasks concurrently.
class ThreadPool {
private:
  // Vector to hold the worker threads.
  std::vector<std::thread> workers;
  
  // Queue to hold the tasks to be executed by the worker threads.
  std::queue<std::function<std::future<void>()>> tasks;
  
  // Mutex to protect access to the task queue.
  std::mutex queueMutex;
  
  // Condition variable to notify worker threads when tasks are available.
  std::condition_variable condition;
  
  // Flag to indicate whether the thread pool is stopping.
  bool stop = false;

public:
  // Constructor to initialize the thread pool with a specified number of threads.
  explicit ThreadPool(size_t threads);
  
  // Method to enqueue a new task into the task queue.
  template <class F> void enqueue(F &&task);
  
  // Method to wait for all tasks to complete.
  void wait();
};

// Method to enqueue a new task into the task queue.
template <class F> void ThreadPool::enqueue(F &&task) {
  {
    // Lock the queue mutex to ensure thread-safe access to the task queue.
    std::unique_lock<std::mutex> lock(queueMutex);
    
    // Add the new task to the queue.
    tasks.emplace(std::forward<F>(task));
  }

  // Notify one of the waiting worker threads that a new task is available.
  condition.notify_one();
}

#endif
