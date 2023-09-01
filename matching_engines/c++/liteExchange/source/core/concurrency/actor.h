#ifndef _ACTOR_H_
#define _ACTOR_H_

#include <string>
#include <memory>
#include <utility>
#include <atomic>
#include "thread.h"

namespace core
{

class Actor : public Thread
{
    public:

        explicit Actor(const std::string& name = "" ) : Thread(name)
        {
            m_isFinishing.store(false);

            TaskPtr task(new Task(&Actor::run, this));
            setTask(std::move(task));
        }

        virtual ~Actor()
        {
            if (isFinishing() == false) // If client forgets to call shutdown, then we do it implicitly
            {
                shutdown();
            }
        }

        virtual void* run() = 0;
        virtual void shutdown() { sendFinishSignal(); join(); }
        virtual bool isFinishing() { return m_isFinishing.load(); }

    protected:

        void sendFinishSignal() { m_isFinishing.store(true); }

    private:

        std::atomic<bool> m_isFinishing;
};

}// namespace

#endif