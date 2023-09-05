#ifndef _PRETTY_EXCEPTION_H_
#define _PRETTY_EXCEPTION_H_

#include <exception>
#include <stdexcept>
#include <string>

#include <core/compiler/preprocessor.h>

#include "console_utility.h"
#include "callstack.h"
#include "environment.h"


inline std::string convertToStdString(const char* str)
{
    return std::string(str);
}

template <typename T>
T convertToStdString(T const& t)
{
    return t;
}

#define EXCEPTION_CALLSTACK_DEPTH 16
#define EXCEPTION_CONSOLE_OUTPUT 1
#define EXCEPTION_CONSOLE_FOREGROUND_COLOR core::ConsoleColor::FG_RED
#define EXCEPTION_MESSAGE_BOX_OUTPUT 0

#if EXCEPTION_CONSOLE_OUTPUT
#define OUTPUT_EXCEPTION_TO_CONSOLE(message) core::consoleOutputWithColor(EXCEPTION_CONSOLE_FOREGROUND_COLOR, message)
#else
#define OUTPUT_EXCEPTION_TO_CONSOLE(message)
#endif

#if EXCEPTION_MESSAGE_BOX_OUTPUT
#if _WIN32
#include <windows.h>
#define OUTPUT_EXCEPTION_TO_MESSAGE_BOX(message) MessageBoxA(nullptr, message, MB_OK, MB_ICONERROR)
#endif
#endif

#ifndef OUTPUT_EXCEPTION_TO_MESSAGE_BOX
#define OUTPUT_EXCEPTION_TO_MESSAGE_BOX
#endif
// For throwing exceptions with messages that contain the file name and the line number info
// We could use boost::exception , however in that we have to catch boost::exception class

// Implemented this "pretty_exception" macro using macro indirection in order to stick with
// std::runtime_error or std::logic_error also to have colored outputs
// Additionaly we have file, line number and callstack information

// Note for MSVC : std::exception implementation of Microsoft has an overloaded constructor
// but this is not standard. Therefore using std::runtime_error

// Macro technique used : http://stackoverflow.com/questions/19343205/c-concatenating-file-and-line-macros
// and http://stackoverflow.com/questions/2670816/how-can-i-use-the-compile-time-constant-line-in-a-string
// Putting a single # before a macro causes it to be changed into a string of its value, instead of its bare value.
// Putting a double ## between two tokens causes them to be concatenated into a single token.
#define STRINGIFY_DETAIL(x) #x
#define STRINGIFY(x) STRINGIFY_DETAIL(x)

#define THROW_PRETTY_EXCEPTION(msg, exceptionType, exceptionTypeAsStringLiteral)  \
      do  {     \
                      { \
                std::string message =  std::string("Exception type : ") + exceptionTypeAsStringLiteral + NEW_LINE + NEW_LINE; \
                message +=  std::string("Message : ") + convertToStdString(msg) + NEW_LINE + NEW_LINE; \
                message += "File : " __FILE__ "Line : " STRINGIFY(__LINE__) NEW_LINE NEW_LINE; \
                message += "Callstack : " NEW_LINE NEW_LINE; \
                message += core::getCallstackAsString(EXCEPTION_CALLSTACK_DEPTH); \
                message += NEW_LINE NEW_LINE; \
                OUTPUT_EXCEPTION_TO_CONSOLE(message.c_str()); \
                OUTPUT_EXCEPTION_TO_MESSAGE_BOX(message.c_str()); \
                throw exceptionType(message.c_str()); \
              } \
          }while(0);



#define THROW_PRETTY_RUNTIME_EXCEPTION(msg) THROW_PRETTY_EXCEPTION(msg, std::runtime_error , "std::runtime_error")
#define THROW_PRETTY_LOGICAL_EXCEPTION(msg) THROW_PRETTY_EXCEPTION(msg, std::logic_error , "std::logic_error")
#define THROW_PRETTY_INVALID_ARG_EXCEPTION(msg) THROW_PRETTY_EXCEPTION(msg, std::invalid_argument, "std::invalid_argument")
#define THROW_PRETTY_LENGTH_EXCEPTION(msg) THROW_PRETTY_EXCEPTION(msg, std::length_error, "std::length_error")

#endif