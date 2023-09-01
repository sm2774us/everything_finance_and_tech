#ifndef _FASTCALL_H_
#define _FASTCALL_H_

#if defined(_MSC_VER)
#if _MSC_VER >= 1800
#define fastcall __fastcall
#else
#define fastcall __vectorcall
#endif
#elif defined(__GNUC__)
#define fastcall __attribute__((fastcall))
#endif

#endif