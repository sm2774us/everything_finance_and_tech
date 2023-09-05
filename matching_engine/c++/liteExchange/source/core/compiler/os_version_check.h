#ifndef _OS_VERSION_CHECK_
#define _OS_VERSION_CHECK_

#if (! defined(__linux__)) && (! defined(_WIN32) )
#error "This project is supported for Linux and Windows systems"
#endif

#endif