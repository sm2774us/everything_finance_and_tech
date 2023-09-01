#ifndef _NO_INLINE_H_
#define _NO_INLINE_H_

#if defined(_MSC_VER)
#define NO_INLINE __declspec(noinline)
#elif defined(__GNUC__)
#define NO_INLINE __attribute__((noinline))
#endif

#endif