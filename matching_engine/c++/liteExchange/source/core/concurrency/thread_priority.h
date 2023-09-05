#ifndef _THREAD_PRIORITY_H
#define _THREAD_PRIORITY_H

#include <array>
#include <string>

#include <core/pretty_exception.h>

#ifdef __linux__
#include <sys/time.h>
#include <sys/resource.h>
#elif _WIN32
#include <process.h>
#include <windows.h>
#endif

namespace core
{

// For Linux we are using nice values rather than scheduler priorities
// as scheduler priorities are not designed for our purpose :
//

enum class ThreadPriority
            {
                IDLE,
                BELOW_NORMAL,
                NORMAL,
                ABOVE_NORMAL,
                CRITICAL
            };

struct ThreadPriorityNode
{
    ThreadPriority priority;
    int value;
};

const static std::array<ThreadPriorityNode, 5> NATIVE_THREAD_PRIORITIES =
{
    //DO POD INITIALISATION
    {
#ifdef __linux__
        ThreadPriority::IDLE, 19,
        ThreadPriority::BELOW_NORMAL, 1,
        ThreadPriority::NORMAL, 0,
        ThreadPriority::ABOVE_NORMAL, -1,
        ThreadPriority::CRITICAL, -20
#elif _WIN32
        ThreadPriority::IDLE, THREAD_PRIORITY_IDLE,    //-15
        ThreadPriority::BELOW_NORMAL, THREAD_PRIORITY_BELOW_NORMAL, // -1
        ThreadPriority::NORMAL, THREAD_PRIORITY_NORMAL, // 0
        ThreadPriority::ABOVE_NORMAL, THREAD_PRIORITY_ABOVE_NORMAL, // 1
        ThreadPriority::CRITICAL, THREAD_PRIORITY_TIME_CRITICAL // 15
#endif
    }
};

inline ThreadPriority getThreadPriorityFromString(const std::string& input)
{
    ThreadPriority ret;

    if( !input.compare("IDLE"))
    {
        ret = ThreadPriority::IDLE;
    }
    else if ( !input.compare("BELOW_NORMAL"))
    {
        ret = ThreadPriority::BELOW_NORMAL;
    }
    else if ( !input.compare("NORMAL"))
    {
        ret = ThreadPriority::NORMAL;
    }
    else if ( !input.compare("ABOVE_NORMAL"))
    {
        ret = ThreadPriority::ABOVE_NORMAL;
    }
    else if ( !input.compare("CRITICAL"))
    {
        ret = ThreadPriority::CRITICAL;
    }
    else
    {
        THROW_PRETTY_INVALID_ARG_EXCEPTION("Invalid thread priority")
    }

    return ret;
}

}// namespace

#endif