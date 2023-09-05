#ifndef _SINGLETON_DCLP_H_
#define _SINGLETON_DCLP_H_

#include <core/noncopyable.h>
#include <core/concurrency/spinlock.hpp>
#include <atomic>
#include <mutex>

/*
    Meyers/Alexandrescu paper about issues with DCLP without memory fences : http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf

    Implementation reference : http://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/
*/

namespace core
{
template <class T>
class SingletonDCLP : public core::NonCopyable
{
    public:
        static T * getInstance()
        {
            T* tmp = m_instance.load(std::memory_order_relaxed);
            std::atomic_thread_fence(std::memory_order_acquire);    // MEMORY-BARRIER SYNC BEGINS
            if (tmp == nullptr)
            {
                std::lock_guard<core::SpinLock> lock(m_lock);
                tmp = m_instance.load(std::memory_order_relaxed);
                if (tmp == nullptr)
                {
                    tmp = new T;
                    std::atomic_thread_fence(std::memory_order_release); // MEMORY-BARRIER SYNC ENDS
                    m_instance.store(tmp, std::memory_order_relaxed);
                }
            }
            return tmp;
        }
    private:
        static core::SpinLock m_lock;
        static std::atomic<T*> m_instance;
};

template <class T>
#if defined(_MSC_VER)
std::atomic<T*> SingletonDCLP<T>::m_instance = nullptr;
#elif defined(__GNUC__)
std::atomic<T*> SingletonDCLP<T>::m_instance{0}; // Atomic types don`t have copy constructors
#endif


template <class T>
core::SpinLock SingletonDCLP<T>::m_lock;

} //namespace

#endif