#ifndef _NOEXCEPT_H_
#define _NOEXCEPT_H_

#if defined(_MSC_VER)
#if (_MSC_VER <= 1800)
#include <xkeycheck.h> // In order to allow keyword macros
#define noexcept throw()
#endif
#endif

#endif