#ifndef _THREAD_POOL_ARGUMENTS_H_
#define _THREAD_POOL_ARGUMENTS_H_

#include <string>
#include <vector>

#include <core/concurrency/thread_priority.h>

namespace core
{

#define DEFAULT_WORK_QUEUE_SIZE 128

struct ThreadPoolArguments
{
    bool m_pinThreadsToCores;
    bool m_hyperThreading;
    int m_workQueueSizePerThread;
    int m_threadStackSize;
    ThreadPriority m_threadPriority;
    std::vector<std::string> m_threadNames;

    ThreadPoolArguments()
    : m_pinThreadsToCores{true}, m_hyperThreading{false}, m_workQueueSizePerThread{ DEFAULT_WORK_QUEUE_SIZE }
    , m_threadStackSize{ 0 }, m_threadPriority{ ThreadPriority::NORMAL }
    {}
};


}//namespace
#endif