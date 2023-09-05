#ifndef _ALIGN_AS_H_
#define _ALIGN_AS_H_

// Use C++11 if available , otherwise compiler extensions
#if defined(_MSC_VER)
#if _MSC_VER<=1800
#include <xkeycheck.h> // In order to allow keyword macros
#define alignas(A) __declspec(align(A))
#endif
#elif defined(__GNUC__)
#if (__GNUC__==4) and (__GNUC_MINOR__ <= 7)
#define alignas(A) __attribute__((aligned(A)))
#endif
#endif

#endif