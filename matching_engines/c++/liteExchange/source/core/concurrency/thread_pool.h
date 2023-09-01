#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <cstddef>
#include <atomic>
#include <vector>
#include <memory>
#include <thread>
#include <vector>

#include <core/noncopyable.h>

#include <core/concurrency/task.h>
#include <core/concurrency/thread.h>
#include <core/concurrency/thread_wait_strategy.h>
#include <core/concurrency/thread_pool_arguments.h>
#include <core/concurrency/ring_buffer_spsc_lockfree.hpp>

namespace core
{

using ThreadPoolQueue = core::RingBufferSPSCLockFree<core::Task>;
using ThreadPoolQueuePtr = std::unique_ptr<ThreadPoolQueue>;

class ThreadPool : public core::NonCopyable, public YieldWaitStrategy
{
    public :
        ThreadPool() : m_numOfThreads(0) { m_isShuttingDown.store(false); }
        ~ThreadPool() { shutdown(); }
        void initialise(const ThreadPoolArguments& args);
        void submitTask(const Task& task, std::size_t queueID);
        void shutdown();

    private:
        struct ThreadArgument
        {
            ThreadPool* m_threadPool = nullptr;
            unsigned int m_queueIndex = -1;
            ThreadArgument(ThreadPool* pool, unsigned int queueIndex) : m_threadPool(pool), m_queueIndex(queueIndex) {}
        };

        // Move ctor deletion
        ThreadPool(ThreadPool&& other) = delete;
        // Move assignment operator deletion
        ThreadPool& operator=(ThreadPool&& other) = delete;

        std::vector<ThreadPtr> m_threads;
        std::vector<ThreadArgument> m_threadArguments;
        std::vector<ThreadPoolQueuePtr>  m_threadQueues;

        unsigned int m_numOfThreads;
        std::atomic<bool> m_isShuttingDown;
        static void* workerThreadFunction(void* argument);
};

}//namespace
#endif