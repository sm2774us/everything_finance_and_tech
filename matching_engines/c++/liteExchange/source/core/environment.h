#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_


#ifdef _WIN32
#define NEW_LINE "\r\n"
#elif __linux__
#define NEW_LINE "\n"
#endif

#include <string>

#if __linux__
#include <errno.h>
#elif _WIN32
#include <windows.h>
#endif

#define MAX_OS_LAST_ERROR_LENGTH 1024

namespace core
{

class Environment
{
    public :

        static std::string getLastError()
        {
            std::string ret;
#ifdef __linux__
            ret = strerror(errno);
#elif _WIN32
            DWORD errCode = GetLastError();
            char *err;
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                nullptr,
                errCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
                (LPTSTR)&err,
                0,
                nullptr);

            ret = err;
#endif
            return ret;
        }
};

}// namespace

#endif