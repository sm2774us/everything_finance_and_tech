#ifndef _CONSOLE_UTILITY_H_
#define _CONSOLE_UTILITY_H_

#include <cstdarg>
#include <iostream>
#include <array>
#include <string>
#include <type_traits>

#if __linux__
#include <string.h>
#elif _WIN32
#include <windows.h>
#endif

#include <core/string_utility.h>

namespace core
{

enum class ConsoleColor // FG:foreground
{
    FG_DEFAULT,
    FG_RED,
    FG_GREEN,
    FG_BLUE
};

struct ConsoleColorNode
{
    ConsoleColor color;
    int value;
};

const std::array<ConsoleColorNode, 4> NATIVE_CONSOLE_COLORS =
{
    //DO POD INITIALISATION
    {
#ifdef __linux__
        // https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
        ConsoleColor::FG_DEFAULT, 0,
        ConsoleColor::FG_RED, 31,
        ConsoleColor::FG_GREEN, 32,
        ConsoleColor::FG_BLUE, 34
#elif _WIN32
        ConsoleColor::FG_DEFAULT, 0,
        ConsoleColor::FG_RED, FOREGROUND_RED,
        ConsoleColor::FG_GREEN, FOREGROUND_GREEN,
        ConsoleColor::FG_BLUE, FOREGROUND_BLUE,
#endif
    }
};

#ifdef __linux__
inline std::string getAnsiColorCode(ConsoleColor color)
{
    std::string ret;
    switch(color)
    {
        case ConsoleColor::FG_DEFAULT:
            ret = "\033[0;31m";
            break;

        case ConsoleColor::FG_RED:
            ret = "\033[0;31m";
            break;

        case ConsoleColor::FG_GREEN:
            ret = "\033[0;32m";
            break;

        case ConsoleColor::FG_BLUE:
            ret = "\033[0;34m";
            break;

        default:
            break;
    }
    return ret;
}
#endif

template <typename... Args>
inline void consoleOutputWithColor(ConsoleColor foregroundColor, const char* message, Args&&... args)
{
    std::string buffer;
    buffer = core::format(message, args...);
#ifdef _WIN32
    auto fgIndex = static_cast<std::underlying_type<ConsoleColor>::type>(foregroundColor);
    auto foreGroundColorCode = NATIVE_CONSOLE_COLORS[fgIndex].value;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    auto setConsoleAttribute = [&hConsole](int code){ SetConsoleTextAttribute(hConsole, code);  };

    if ( foregroundColor != ConsoleColor::FG_DEFAULT)
    {
        setConsoleAttribute(foreGroundColorCode | FOREGROUND_INTENSITY);
    }
    FlushConsoleInputBuffer(hConsole);
    std::cout << buffer << std::endl;
    SetConsoleTextAttribute(hConsole, 15); //set back to black background and white text
#elif __linux__
    std::cout << getAnsiColorCode(foregroundColor)<< buffer << "\033[0m" << std::endl;
#endif
}

inline void pressAnyKeyToContinue()
{
#ifdef _WIN32
    system("pause");
#elif __linux__
    std::cout << std::endl << "Press any key to continue..." << std::endl;
    system("read");
#endif
}

}// namespace

#endif