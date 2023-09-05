#ifndef _TRACE_H_
#define _TRACE_H_

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <syslog.h>
#include <unistd.h>
#endif

#include <string>
#include <core/string_utility.h>

namespace core
{

// For Linux , we are tracing to syslog , they can be seen via tail -f /var/log/messages
// For Windows , traces can be seen with Microsoft`s DbgView : https://technet.microsoft.com/en-us/sysinternals/debugview.aspx?f=255&MSPPError=-2147217396

#define MAX_TRACE_MESSAGE_LENGTH 1024

template <typename... Args>
inline void trace(const char* message, Args&&... args)
{
    std::string buffer;
    buffer = core::format(message, args...);

#ifdef _WIN32
    OutputDebugStringA(buffer.c_str());
    OutputDebugStringA(NEW_LINE);
#elif __linux__
    openlog("slog", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, buffer.c_str() );
    closelog();
#endif
}

}// namespace

#endif