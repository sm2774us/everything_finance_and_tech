#ifndef RAPID_TRADER_THREAD_JOINER_H
#define RAPID_TRADER_THREAD_JOINER_H
#include <vector>
#include <thread>

namespace RapidTrader::Concurrent {
/**
 * A class to clean up threads used by the thread pool.
 */
struct ThreadJoiner
{
    explicit ThreadJoiner(std::vector<std::thread> &threads_)
        : threads(threads_)
    {}

    ~ThreadJoiner()
    {
        // Join all joinable threads.
        for (auto &thread : threads)
        {
            if (thread.joinable())
                thread.join();
        }
    }

private:
    // The threads that the joiner is responsible for joining.
    std::vector<std::thread> &threads;
};
} // namespace RapidTrader::Concurrent
#endif // RAPID_TRADER_THREAD_JOINER_H
