#ifndef RAPID_TRADER_QUEUE_H
#define RAPID_TRADER_QUEUE_H
#include <queue>
#include <mutex>
#include <condition_variable>

namespace RapidTrader::Concurrent {
/**
 * A thread-safe queue.
 *
 * @tparam T type of the objects that will be stored in the queue.
 */
template<typename T>
class Queue
{
public:
    Queue() = default;

    Queue(const Queue &other)
    {
        std::lock_guard<std::mutex> lk(m);
        queue = other.queue;
    }

    void push(T data)
    {
        std::lock_guard<std::mutex> lk(m);
        queue.push(data);
        c.notify_one();
    }

    T waitAndPop()
    {
        std::unique_lock<std::mutex> lk(m);
        c.wait(lk, [this] { return !queue.empty(); });
        auto data = queue.front();
        queue.pop();
        return data;
    }

    std::unique_ptr<T> tryPop()
    {
        std::unique_lock<std::mutex> lk(m);
        if (queue.empty())
            return std::make_unique<T>();
        auto data = std::make_unique<T>(queue.front());
        queue.pop();
        return data;
    }

    bool tryPop(T &data)
    {
        std::lock_guard<std::mutex> lk(m);
        if (queue.empty())
            return false;
        data = std::move(queue.front());
        queue.pop();
        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(m);
        return queue.empty();
    }

private:
    mutable std::mutex m;
    std::queue<T> queue;
    std::condition_variable c;
};
} // namespace RapidTrader::Concurrent
#endif // RAPID_TRADER_QUEUE_H
