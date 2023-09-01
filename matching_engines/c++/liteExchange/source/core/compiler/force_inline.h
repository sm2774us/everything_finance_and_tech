#ifndef _FORCE_INLINE_H_
#define _FORCE_INLINE_H_

#if defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#elif defined(__GNUC__)
#define FORCE_INLINE __attribute__((always_inline))
#endif

#endif