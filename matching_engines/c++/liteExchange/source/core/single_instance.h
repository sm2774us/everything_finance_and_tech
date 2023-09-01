#ifndef _SINGLE_INSTANCE_H_
#define _SINGLE_INSTANCE_H_

#include <cstdint>
#include <core/noncopyable.h>

#ifdef __linux__
#include <string>
#elif _WIN32
#include <windows.h>
#endif

namespace core
{

#define SINGLE_INSTANCE_LOCK "single_instance_lock"

class SingleInstance : public core::NonCopyable
{
    public:

        SingleInstance();
        ~SingleInstance();

        // Move ctor deletion
        SingleInstance(SingleInstance&& other) = delete;
        // Move assignment operator deletion
        SingleInstance& operator=(SingleInstance&& other) = delete;

        bool operator()();

    private:

#ifdef __linux__
        int m_fd;
        std::string m_filePath;
#elif _WIN32
        HANDLE m_mutex;
        unsigned long  m_lastError;
#endif
};

}//namespace
#endif