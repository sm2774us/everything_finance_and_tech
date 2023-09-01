#ifndef _CALLSTACK_H_
#define _CALLSTACK_H_

// Note for Linux to make Linux one work ,
// add -rdynamic to linker options :
// https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html :
/*
    -rdynamic : Pass the flag -export-dynamic to the ELF linker, on targets that support it.
    This instructs the linker to add all symbols, not only used ones, to the dynamic symbol table.
    This option is needed for some uses of dlopen or to allow obtaining backtraces from within a program.
*/

#include <cassert>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <cstdlib>

#include "environment.h"

namespace core
{

#ifdef __linux__
#include <execinfo.h>
#elif _WIN32
#include <windows.h>
#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")

    class SymbolHandler
    {
    private:
        HANDLE m_process;
    public:
        explicit SymbolHandler(HANDLE process) : m_process{ process }
        {
            SymInitialize(m_process, nullptr, TRUE);
        }

        ~SymbolHandler()
        {
            SymCleanup(m_process);
        }
    };
#endif

#define MAX_FRAME_NUMBER 128
#define DEFAULT_FRAME_NUMBER 32
#define MAX_SYMBOL_LENGTH 255

inline std::vector<std::string> getCallStack(int frameNumber = DEFAULT_FRAME_NUMBER)
{
    assert(MAX_FRAME_NUMBER >= frameNumber);
    std::vector<std::string> ret;
    ret.reserve(frameNumber);
#ifdef __linux__
    void *frames[MAX_FRAME_NUMBER];
    int frameCount{0};
    char **messages = (char **)nullptr;

    frameCount = backtrace(frames, frameNumber);
    messages = backtrace_symbols(frames, frameCount);

    //Rather then zero we are starting from 1 to exclude call to this function
    for (unsigned short i{ 1 }; i < frameCount; ++i)
    {
        std::stringstream currentFrame;
        currentFrame << frameCount - i << " : " << messages[i];
        ret.push_back(currentFrame.str());
    }

#elif _WIN32
    HANDLE process;
    process = GetCurrentProcess();
    SymbolHandler symbolHandler{ process };

    std::unique_ptr<SYMBOL_INFO, decltype(free)*> symbol{ (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + (MAX_SYMBOL_LENGTH + 1)* sizeof(char), 1), std::free};

    symbol->MaxNameLen = MAX_SYMBOL_LENGTH;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    void* stack[MAX_FRAME_NUMBER];
    auto frameCount = CaptureStackBackTrace(1, frameNumber, stack, nullptr);

    //Rather then zero we are starting from 1 to exclude call to this function
    for (unsigned short i{ 1 }; i < frameCount; ++i)
    {
        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol.get());
        std::stringstream currentFrame;
        currentFrame << frameCount - i << " : " << symbol->Name << " - " << symbol->Address;
        ret.push_back(currentFrame.str());
    }
#endif
    return ret;
}

inline std::string getCallstackAsString(int frameNumber = DEFAULT_FRAME_NUMBER)
{
    std::string ret;
    auto callstack = getCallStack(frameNumber);
    for (auto& frame : callstack)
    {
        ret += frame;
        ret += NEW_LINE;
    }
    return ret;
}

}// namespace

#endif