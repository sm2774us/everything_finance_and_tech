#ifndef _THREAD_WAIT_STRATEGY_H_
#define _THREAD_WAIT_STRATEGY_H_

#include <core/concurrency/thread.h>

namespace core
{

class ThreadWaitStrategy
{
    public :
        virtual void applyWaitStrategy(unsigned long argument) = 0;
};

class SleepWaitStrategy : public ThreadWaitStrategy
{
    public :
        virtual void applyWaitStrategy(unsigned long argument) override
        {
            core::Thread::sleep(argument);
        }
};

class YieldWaitStrategy : public ThreadWaitStrategy
{
    public:
        virtual void applyWaitStrategy(unsigned long argument) override
        {
            core::Thread::yield();
        }
};

class BusyWaitStrategy : public ThreadWaitStrategy
{
    public:
        virtual void applyWaitStrategy(unsigned long argument) override
        {
            return; // Returns immediately
        }
};

}// namespace

#endif