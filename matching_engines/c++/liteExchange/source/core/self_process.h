#ifndef _SELF_PROCESS_H_
#define _SELF_PROCESS_H_

#include <string>
#include <cstdlib>
#include <array>

#if __linux__
#include <unistd.h>
#include <sys/resource.h>
#elif _WIN32
#include <direct.h>
#include <windows.h>
#endif

#include <core/pretty_exception.h>
#include <core/file_utility.h>
#include <core/string_utility.h>

namespace core
{

enum class ProcessPriority
{
    IDLE,
    BELOW_NORMAL,
    NORMAL,
    ABOVE_NORMAL,
    CRITICAL
};

struct ProcessPriorityNode
{
    ProcessPriority priority;
    int value;
};

const static std::array<ProcessPriorityNode, 5> NATIVE_PROCESS_PRIORITIES =
{
    //DO POD INITIALISATION
    {
#ifdef __linux__
        ProcessPriority::IDLE, 19,
        ProcessPriority::BELOW_NORMAL, 1,
        ProcessPriority::NORMAL, 0,
        ProcessPriority::ABOVE_NORMAL, -1,
        ProcessPriority::CRITICAL, -20
#elif _WIN32
        ProcessPriority::IDLE, IDLE_PRIORITY_CLASS,
        ProcessPriority::BELOW_NORMAL, BELOW_NORMAL_PRIORITY_CLASS,
        ProcessPriority::NORMAL, NORMAL_PRIORITY_CLASS,
        ProcessPriority::ABOVE_NORMAL, ABOVE_NORMAL_PRIORITY_CLASS,
        ProcessPriority::CRITICAL, REALTIME_PRIORITY_CLASS
#endif
    }
};

class SelfProcess
{
    public :

#if ! defined(UNIT_TEST)
        static void setCurrentWorkingDirectory(const std::string& dir)
        {
#if _WIN32
            _chdir(dir.c_str());
#elif __linux__
            chdir(dir.c_str());
#endif
        }
#endif

        static std::string getCurrentExecutableDirectory()
        {
            std::string ret;
            const std::size_t tempBufferLength = 1024;
            char tempBuffer[tempBufferLength] = { char(NULL) };
#ifdef __linux__
            std::string procAddress;
            procAddress = core::format("/proc/%d/exe", getpid());
            int bytes = readlink(procAddress.c_str(), tempBuffer, tempBufferLength);
            tempBuffer[bytes] = '\0';
#elif _WIN32
            GetModuleFileName(NULL, tempBuffer, tempBufferLength);
#endif
            ret = tempBuffer;
            ret = core::getDirectoryNameFromPath(ret);
            return ret;
        }

        static std::string getCurrentWorkingDirectory()
        {
            std::string ret;
            char* temp = getenv("PATH");
            ret = temp;
            return ret;
        }

        static bool amIAdmin()
        {
            bool ret = false;
#ifdef __linux__
            if (!getuid())
            {
                ret = true;
            }
#elif _WIN32
#if ! _DEBUG
            BOOL fRet = FALSE;
            HANDLE hToken = NULL;
            if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
            {
                TOKEN_ELEVATION Elevation;
                DWORD cbSize = sizeof(TOKEN_ELEVATION);
                if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize))
                {
                    fRet = Elevation.TokenIsElevated;
                }
            }
            if (hToken)
            {
                CloseHandle(hToken);
            }

            ret = fRet == TRUE ? true : false;
#else
            ret = true;
#endif
#endif
            return ret;
        }

        static bool setPriority(ProcessPriority priority)
        {
            bool ret{false};
            int index = static_cast<std::underlying_type<ProcessPriority>::type >(priority);
            auto nativePriorityValue = NATIVE_PROCESS_PRIORITIES[index].value;
#ifdef __linux__
            if( setpriority(PRIO_PROCESS, getpid(), nativePriorityValue) != -1 )
            {
                ret = true;
            }
#elif _WIN32
            if (::SetPriorityClass(GetCurrentProcess(), nativePriorityValue) != 0)
            {
                ret = true;
            }
#endif
            return ret;
        }

        static int getPriority()
        {
            int ret{ -1 };
#ifdef __linux__
            ret = getpriority(PRIO_PROCESS, getpid());
#elif _WIN32
            ret = GetPriorityClass(GetCurrentProcess());
#endif
            return ret;
        }

        static ProcessPriority getProcessPriorityFromString(const std::string& input)
        {
            ProcessPriority ret;

            if (!input.compare("IDLE"))
            {
                ret = ProcessPriority::IDLE;
            }
            else if (!input.compare("BELOW_NORMAL"))
            {
                ret = ProcessPriority::BELOW_NORMAL;
            }
            else if (!input.compare("NORMAL"))
            {
                ret = ProcessPriority::NORMAL;
            }
            else if (!input.compare("ABOVE_NORMAL"))
            {
                ret = ProcessPriority::ABOVE_NORMAL;
            }
            else if (!input.compare("CRITICAL"))
            {
                ret = ProcessPriority::CRITICAL;
            }
            else
            {
                THROW_PRETTY_INVALID_ARG_EXCEPTION("Invalid process priority")
            }

            return ret;
        }

};

}// namespace

#endif