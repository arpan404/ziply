#ifndef CONCURRENCY_HPP
#define CONCURRENCY_HPP

class Concurrency
{
public:
    explicit Concurrency(size_t concurrency);
    template <typename F>
    void enqueue(F &&f);

    ~Concurrency();

private:
    // Currency - 0 is only-cpu mode, and 1 is cpu-gpu mode
    // only-cpu mode utilizes all cores of cpu, and cpu-gpu mode utilizes all cores of cpu and gpu
    int concurrency;
    std::mutex queueMutex;
    std::queue<std::function<void()>> tasks;
    bool stop = false;
    std::vector<std::thread> workers;
    std::condition_variable condition;
};

Concurrency::Concurrency(size_t concurrency) : concurrency(concurrency);

template <class F>
void Concurrency::enqueue(F &&task)
{
    // TODO: Implement concurrency
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(std::forward<F>(task));
    }
    condition.notify_one();
}

#endif