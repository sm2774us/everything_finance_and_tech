#ifndef _CPP_VERSION_CHECK_
#define _CPP_VERSION_CHECK_

#if defined(_MSC_VER)
// __cplusplus should be 201103L
// however this is not fully implemented in MSVC :
// http://stackoverflow.com/questions/14131454/visual-studio-2012-cplusplus-and-c-11
#if __cplusplus < 199711L
#error "This project requires to be compiled for C++11"
#endif
#elif defined(__GNUC__)
#if __cplusplus < 201103L
#error "This project requires to be compiled for C++11"
#endif
#else
#error "Non supported C++ compiler , please use either MSVC or GCC"
#endif

#endif