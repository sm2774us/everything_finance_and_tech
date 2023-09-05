#ifndef _STACK_MEMORY_
#define _STACK_MEMORY_

#include <cstddef>
#include <new>

namespace core
{

class StackOnly
{
    private:
        void *operator new(std::size_t);
        void *operator new[](std::size_t);
        void* operator new  (std::size_t count, const std::nothrow_t& tag);
        void* operator new[](std::size_t count, const std::nothrow_t& tag);

        void operator delete  (void* ptr);
        void operator delete[](void* ptr);
        void operator delete  (void* ptr, const std::nothrow_t& tag);
        void operator delete[](void* ptr, const std::nothrow_t& tag);
        void operator delete  (void* ptr, std::size_t sz);
        void operator delete[](void* ptr, std::size_t sz);
};

}

#endif