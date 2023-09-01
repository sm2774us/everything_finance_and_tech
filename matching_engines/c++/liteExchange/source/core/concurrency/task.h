#ifndef _TASK_H_
#define _TASK_H_

#include <memory>
#include <functional>
#include <string>
#include <core/memory/heap_memory_cache_aligned_allocators.hpp>

namespace core
{

using TaskReturnType = void*;

class Task : public core::Aligned<>
{
    public :
        // Constructor
        Task() = default;
        // Copy constructor
        Task(const Task& rhs) = default;
        // Assignment operator
        Task& operator= (const Task &rhs) = default;
        // Destructor
        ~Task() = default;

        template<typename Function, typename ...Args>
        Task(Function f, Args... args) : m_callback{std::bind(f, args...)}
        { }

        void execute()
        {
            try
            {
                // Execute callback & Transfer return value buffer to Task via void* return type
                m_returnValue = m_callback();
            }
            catch (...)
            {
                m_taskExceptionPtr = std::current_exception();
            }
        }

        TaskReturnType getReturnValue()const
        {
            return m_returnValue;
        }

        std::exception_ptr getException() const
        {
            return m_taskExceptionPtr;
        }

    private:
        std::function<void*()> m_callback;
        std::exception_ptr m_taskExceptionPtr = nullptr;
        TaskReturnType m_returnValue;
};

using TaskPtr = std::unique_ptr<Task>;

}//namespace

#endif