#ifndef _HEAP_MEMORY_CACHE_ALIGNED_ALLOCATORS_
#define _HEAP_MEMORY_CACHE_ALIGNED_ALLOCATORS_

#include <cstddef>
#include <type_traits>
#include <new>
#include <limits>
#include <stdexcept>

#include <core/compiler/noexcept.h>
#include <core/pretty_exception.h>

#include "cpu_memory.h"
#include "virtual_memory.h"

#define IS_POWER_OF_TWO(n) ( n && ((n & (n - 1)) == 0) ? true : false )

namespace core
{

static void* alignedMalloc(std::size_t size, std::size_t alignment) throw(std::bad_alloc)
{
    void* ptr{ nullptr };
#ifdef __linux__
    posix_memalign(&ptr, alignment, size);
#elif _WIN32
    ptr = _aligned_malloc(size, alignment);
#endif
    //For just C++11 implementation with std::align
    //See http://en.cppreference.com/w/cpp/core/memory/align

    if (ptr == nullptr)
    {
        throw std::bad_alloc();
    }

    return ptr;
}

static void alignedFree(void* ptr) noexcept
{
#ifdef __linux__
    free(ptr);
#elif _WIN32
    _aligned_free(ptr);
#endif
}

template<std::size_t alignment = CACHE_LINE_SIZE>
class Aligned
{
public:

    Aligned()
    {
        static_assert(IS_POWER_OF_TWO(alignment), "Template argument must be a power of two.");
    }

    /*
    From Herb Sutter`s exceptional C++

    All flavors of operator new() and operator delete() are always static functions, even if
    they're not declared static. Although C++ doesn't force you to say "static" explicitly when
    you declare your own, it's better to do so anyway, because it serves as a reminder to yourself as
    you're writing the code and as a reminder to the next programmer who has to maintain it.
    */

    static void* operator new(std::size_t size) throw(std::bad_alloc)
    {
        void* ret = nullptr;

        ret = alignedMalloc(size, alignment);

        return ret;
    }

    static void operator delete(void* ptr) noexcept
    {
        alignedFree(ptr);
    }


};

template<typename T>
class AlignedContainerPolicy
{
public:
    AlignedContainerPolicy()
    {
        static_assert(std::is_fundamental<T>::value || std::is_base_of<core::Aligned<>, T>::value, "Allowed concurrent container specialisations : \
                                                                                                                                                                                                                                                                                                               Classes derived from core::Aligned, fundamental types");
    }
};

template <typename T, std::size_t alignment = CACHE_LINE_SIZE>
class AlignedAllocator
{
    public:
        // The following will be the same for virtually all allocators.
        using pointer = T*;
        using const_pointer = const T *;
        using reference = T&;
        using const_reference = const T&;
        using value_type = T;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        // Default constructor, copy constructor, rebinding constructor, and destructor.
        // Empty for stateless allocators.
        AlignedAllocator()
        {
            static_assert(IS_POWER_OF_TWO(alignment), "Template argument must be a power of two.");
        }
        AlignedAllocator(const AlignedAllocator&) { }

        template <typename U> AlignedAllocator(const AlignedAllocator<U>&) { }
        ~AlignedAllocator() { }

        T* address(T& r) const noexcept
        {
            return &r;
        }

            const T * address(const T& s) const noexcept
        {
            return &s;
        }

        size_t max_size() const noexcept
        {
            // The following has been carefully written to be independent of
            // the definition of size_t and to avoid signed/unsigned warnings.

            // Extra parantesis are needed for MSVC because Windows headers define min max
            // See http://stackoverflow.com/questions/1904635/warning-c4003-and-errors-c2589-and-c2059-on-x-stdnumeric-limitsintmax
            return (std::numeric_limits<int>::max)() / sizeof(value_type);
        }

        // The following must be the same for all allocators.
        template <typename U>
        struct rebind
        {
            typedef AlignedAllocator<U> other;
        };

        bool operator!=(const AlignedAllocator& other) const
        {
            return !(*this == other);
        }

        void construct(T * const p, const T& t) const
        {
            void * const pv = static_cast<void *>(p);
            new (pv)T(t);
        }

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter
#endif
        void destroy(T * const p) const
        {
            p->~T();
        }

#ifdef _MSC_VER
#pragma warning(pop)
#endif

        // Returns true if and only if storage allocated from *this
        // can be deallocated from other, and vice versa.
        // Always returns true for stateless allocators.
        bool operator==(const AlignedAllocator& other) const
        {
            return true;
        }

        // The following will be different for each allocator.
        T * allocate(const std::size_t n) const throw(std::runtime_error, std::length_error)
        {

            // The return value of allocate(0) is unspecified.
            // AlignedAllocator returns NULL in order to avoid depending
            // on malloc(0)’s implementation-defined behavior
            // (the implementation can define malloc(0) to return NULL,
            // in which case the bad_alloc check below would fire).
            // All allocators can return NULL in this case.
            if (n == 0)
            {
                return nullptr;
            }

            // All allocators should contain an integer overflow check.
            // The Standardization Committee recommends that std::length_error
            // be thrown in the case of integer overflow.
            if (n > max_size())
            {
                THROW_PRETTY_LENGTH_EXCEPTION("AlignedAllocator<T>::allocate() – Integer overflow.")
            }

            void * const pv = alignedMalloc(n * sizeof(T), alignment);

            // Allocators should throw std::bad_alloc in the case of memory allocation failure.
            if (pv == nullptr)
            {
                throw std::bad_alloc();
            }

            return static_cast<T *>(pv);
        }

        void deallocate(T * const p, const size_t n) const
        {
            alignedFree(p);
        }

        // The following will be the same for all allocators that ignore hints.
        template <typename U> T * allocate(const size_t n, const U * ) const
        {
            return allocate(n);
        }

        // NOTE FOR MSVC :
        // Allocators are not required to be assignable, so
        // all allocators should have a private unimplemented
        // assignment operator. Note that this will trigger the
        // off-by-default (enabled under /Wall) warning C4626
        // “assignment operator could not be generated because a
        // base class assignment operator is inaccessible” within
        // the STL headers, but that warning is useless.

    private:

        AlignedAllocator& operator=(const AlignedAllocator&);

};

}

#endif