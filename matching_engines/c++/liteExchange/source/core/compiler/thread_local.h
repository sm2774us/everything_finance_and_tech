#ifndef _THREAD_LOCAL_H_
#define _THREAD_LOCAL_H_

/*
    TLS REQUIREMENTS :

    1. GCC : https://gcc.gnu.org/onlinedocs/gcc-3.3.1/gcc/Thread-Local.html

    2. MSVC : https://msdn.microsoft.com/en-us/library/aa295782(v=vs.60).aspx

    C++11 thread_local :

    http://en.cppreference.com/w/cpp/keyword/thread_local
*/

#if defined(_MSC_VER)
#if MSC_VER<=1800
#include <xkeycheck.h> // In order to allow keyword macros
#define thread_local __declspec(thread)
#endif
#elif defined(__GNUC__)
#define thread_local __thread
#endif

#endif