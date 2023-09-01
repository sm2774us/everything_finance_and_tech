#ifndef _CPU_MEMORY_
#define _CPU_MEMORY_

#include <cstddef>
#include <type_traits>

#include <core/compiler/alignas.h>

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <cstdlib>
#include <windows.h>
#include <memory>
#endif

#define CACHE_LINE_SIZE 64
#define CACHE_ALIGNED alignas(CACHE_LINE_SIZE)

namespace core
{

inline std::size_t getCPUCacheLineSize()
{
    std::size_t lineSize{ 0 };
#ifdef __linux__
    lineSize = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
#elif _WIN32
    DWORD bufferSize{ 0 };
    GetLogicalProcessorInformation(0, &bufferSize);

    std::unique_ptr<SYSTEM_LOGICAL_PROCESSOR_INFORMATION, decltype(free)*> buffer{
        (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(bufferSize), free
    };

    GetLogicalProcessorInformation(&buffer.get()[0], &bufferSize);

    for (DWORD i{ 0 }; i != bufferSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i)
    {
        if (buffer.get()[i].Relationship == RelationCache && buffer.get()[i].Cache.Level == 1)
        {
            lineSize = buffer.get()[i].Cache.LineSize;
            break;
        }
    }
#endif
    return lineSize;
}

template <typename T>
inline T getCPUWordSize()
{
    static_assert(std::is_integral<T>::value, "getCPUWordSize : Only numeric values can be used in specialisation");

#if defined(_MSC_VER)
#ifdef _M_IX86
    T architecture = 4;
#else
    T architecture = 8;
#endif
#elif defined(__GNUC__)
#ifdef __i386__
    T architecture = 4;
#else
    T architecture = 8;
#endif
#else
#error "memory_utilities.h : Non supported architecture"
#endif

    return architecture;
}

template <typename T>
inline T alignToTargetCPUWordSize(T input)
{
    static_assert(std::is_integral<T>::value, "alignToTargetCPUWordSize : Only numeric values can be used in specialisation");

    T alignment = getCPUWordSize<T>();
    T alignedNumber = input;

    while (alignedNumber % alignment != 0)
    {
        alignedNumber += 1;
    }

    return alignedNumber;
}

}

#endif