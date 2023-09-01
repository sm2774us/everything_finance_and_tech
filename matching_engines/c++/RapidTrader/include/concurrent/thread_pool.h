#ifndef RAPID_TRADER_THREAD_POOL_H
#define RAPID_TRADER_THREAD_POOL_H
#include <thread>
#include <vector>
#include <atomic>
#include <cassert>
#include <future>
#include "concurrent/thread_joiner.h"
#include "concurrent/queue.h"

namespace RapidTrader::Concurrent {
class ThreadPool
{
public:
    /**
     * A constructor for the thread pool.
     *
     * @param num_threads_ the number of worker threads that will be spawned by
     *                     the thread pool, require that num_threads_ is positive.
     */
    explicit ThreadPool(uint32_t num_threads_ = std::thread::hardware_concurrency())
        : num_threads(num_threads_)
        , running(true)
        , thread_joiner(threads)
    {
        assert(num_threads > 0 && "Thread pool requires at least one thread!");

        thread_queues.reserve(num_threads);
        threads.reserve(num_threads);

        // Try to spawn the threads for the pool.
        try
        {
            for (auto i = 0; i < num_threads; ++i)
            {
                thread_queues.push_back(std::make_unique<Queue<std::function<void()>>>());
                threads.emplace_back(&ThreadPool::workerThread, this, i);
            }
        }
        catch (...)
        {
            running = false;
            throw;
        }
    }

    /**
     * Submit a void returning task to the thread pool for execution with
     * zero or more arguments.
     *
     * @tparam F a callable type.
     * @tparam Args the arguments to the callable type.
     * @param queue_index the index of the queue that the task will be submitted to,
     *                    require that 0 <= queue_index < number of workers.
     * @param f the function that will submitted to thread pool for execution.
     * @param args zero or more arguments for the function that will be executed.
     */
    template<typename F, typename... Args>
    void submitTask(uint32_t queue_index, F &&f, Args &&...args)
    {
        std::function<void()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        thread_queues[queue_index]->push(task);
    }

    /**
     * Submits a void or non-void returning task that can be waited on to the thread pool
     * for execution with zero or more arguments.
     *
     * @tparam F a callable type
     * @tparam Args the arguments to the callable type.
     * @tparam R the return type of the callable type.
     * @param queue_index the index of the queue that the task will be submitted to, require that
     *                    0 <= queue_index < number of workers.
     * @param f the function that will be executed.
     * @param args zero or more arguments for the function that will be executed.
     * @return a future containing the return value of the executed function.
     */
    template<typename F, typename... Args, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
    std::future<R> submitWaitableTask(uint32_t queue_index, F &&f, Args &&...args)
    {
        std::function<R()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto task_promise = std::make_shared<std::promise<R>>(std::promise<R>());
        thread_queues[queue_index]->push([=] {
            try
            {
                if constexpr (std::is_void_v<R>)
                {
                    std::invoke(task);
                    task_promise->set_value();
                }
                else
                {
                    task_promise->set_value(std::invoke(task));
                }
            }
            catch (...)
            {
                task_promise->set_exception(std::current_exception());
            }
        });
        return task_promise->get_future();
    }

    /**
     * Starts the thread pool, require that the thread pool is not already started.
     */
    void start()
    {
        assert(!running && "Thread pool is already started!");
        running = true;
        // Try to spawn the threads for the pool.
        try
        {
            for (auto i = 0; i < num_threads; ++i)
                threads[i] = std::thread(&ThreadPool::workerThread, this, i);
        }
        catch (...)
        {
            running = false;
            throw;
        }
    }

    /**
     * Stops the thread pool, require that the thread pool has been started.
     */
    void stop()
    {
        assert(running && "Thread pool has not been started!");
        running = false;
    }

    /**
     * @return true if the thread pool has been started and false otherwise.
     */
    bool isStarted()
    {
        return running;
    }

    /**
     * @return number of worker threads the thread pool is using.
     */
    [[nodiscard]] uint32_t numberOfThreads() const
    {
        return num_threads;
    }

    /**
     * A destructor for the thread pool. Notifies worker threads that they are finished.
     */
    ~ThreadPool()
    {
        running = false;
    };

private:
    /**
     * Wait for and execute incoming tasks.
     *
     * @param queue_id the queue ID that the worker thread should
     *                 draw tasks from, require that 0 <= queue < num_threads.
     */
    void workerThread(uint32_t queue_id)
    {
        assert(queue_id < thread_queues.size() && "Invalid queue index!");
        // Keep processing tasks until the running flag is set to false and the queue is empty.
        // Otherwise, there may be unfinished tasks left in the queue.
        while (running || !thread_queues[queue_id]->empty())
        {
            std::function<void()> task;
            bool empty = false;
            if (thread_queues[queue_id]->tryPop(task))
                task();
            else
                std::this_thread::yield();
        }
    }

    // IMPORTANT: The running flag and the work queues must be declared
    // before the vector containing the threads. Otherwise, the queue and
    // the flag would be destroyed before the threads have finished their work.

    // Indicates whether the working threads are running.
    std::atomic_bool running;
    // The number of worker threads in the thread pool - must be at least 1.
    uint32_t num_threads;
    // Thread-safe queue for each worker thread.
    std::vector<std::unique_ptr<Queue<std::function<void()>>>> thread_queues;
    // The worker threads and their corresponding queues - each thread gets its own queue.
    std::vector<std::thread> threads;
    // Handles cleaning up the worker threads when the thread pool is destroyed.
    ThreadJoiner thread_joiner;
};
} // namespace RapidTrader::Concurrent
#endif // RAPID_TRADER_THREAD_POOL_H
