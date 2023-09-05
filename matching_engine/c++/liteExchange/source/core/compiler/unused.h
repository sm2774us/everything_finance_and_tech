#ifndef _UNUSED_H_
#define _UNUSED_H_

//To avoid unused variable warnings
#if defined(__GNUC__)
#define UNUSED(x) (void)(x)
#elif defined(_MSC_VER)
#define UNUSED(x) __pragma(warning(suppress:4100)) x
#endif

#endif