#include "threadpool.hpp"

// Constructor for the ThreadPool class
// Initializes a specified number of worker threads that will process tasks from the task queue.
ThreadPool::ThreadPool(size_t threads) {
  // Create and launch worker threads
  for (size_t i = 0; i < threads; ++i) {
    workers.emplace_back([this]() {
      // Infinite loop to keep the worker thread alive
      while (true) {
        std::function<std::future<void>()> task; // Variable to hold the task to be executed
        {
          // Lock the mutex to safely access the task queue
          std::unique_lock<std::mutex> lock(queueMutex);
          // Wait until there are tasks to process or the pool is stopping
          condition.wait(lock, [this] { return stop || !tasks.empty(); });
          // If the pool is stopping and there are no tasks, exit the thread
          if (stop && tasks.empty()) {
            return;
          }
          // Move the task from the queue to the local variable
          task = std::move(tasks.front());
          tasks.pop(); // Remove the task from the queue
        }
        task(); // Execute the task
      }
    });
  }
}

// Waits for all tasks to complete and stops the thread pool
void ThreadPool::wait() {
  {
    // Lock the mutex to safely modify the stop flag
    std::unique_lock<std::mutex> lock(queueMutex);
    stop = true; // Set the stop flag to true
  }
  condition.notify_all(); // Notify all worker threads to wake up
  // Join all worker threads to ensure they finish execution
  for (std::thread &worker : workers) { worker.join(); }
}
