#include "threadpool.hpp"

// Constructor for the ThreadPool class
// Initializes a specified number of worker threads to process tasks from the queue.
ThreadPool::ThreadPool(size_t threads) {
  // Create and launch worker threads
  for (size_t i = 0; i < threads; ++i) {
    workers.emplace_back([this]() {
      // Infinite loop to keep the worker thread running
      while (true) {
        std::function<std::future<void>()> task; // Variable to hold the task to execute
        {
          // Lock the mutex to safely access the task queue
          std::unique_lock<std::mutex> lock(queueMutex);
          // Wait for a task or a stop signal
          condition.wait(lock, [this] { return stop || !tasks.empty(); });
          // Exit thread if stopping and no tasks are left
          if (stop && tasks.empty()) {
            return;
          }
          // Get the task from the queue
          task = std::move(tasks.front());
          tasks.pop(); // Remove the task from the queue
        }
        task(); // Execute the task
      }
    });
  }
}

// Waits for all tasks to complete and shuts down the thread pool
void ThreadPool::wait() {
  {
    // Lock mutex to safely modify the stop flag
    std::unique_lock<std::mutex> lock(queueMutex);
    stop = true; // Mark the pool as stopping
  }
  condition.notify_all(); // Wake up all worker threads
  // Wait for all threads to finish execution
  for (std::thread &worker : workers) { worker.join(); }
}
