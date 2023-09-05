#ifndef _DATE_TIME_UTILITY_H_
#define _DATE_TIME_UTILITY_H_

#include <string>
#include <chrono>
#include <sstream>
#include <ctime>
#include <cstddef>
#include <iomanip>
#include "string_utility.h"

#if __linux__
#include <sys/time.h>
#endif

// Could use anonymous namespace or static keyword since C++11 removed deprecation
// Even though functions here don`t operate on static data
// preferred inline to avoid code bloat

namespace core
{

enum class Subseconds
{
    SECONDS,
    MILLISECONDS,
    MICROSECONDS
};

inline long long getMilliseconds()
{
    long long ret{ 0 };
#if defined( _MSC_VER ) || ( __GNUC__ > 4 )
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    ret = ms.count();
#else
    timeval curTime;
    gettimeofday(&curTime, NULL);
    ret = curTime.tv_usec / 1000;
#endif
    return ret;
}

inline long long getMicroseconds()
{
    long long ret{ 0 };
#if defined( _MSC_VER ) || ( __GNUC__ > 4 )
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
    ret = ms.count();
#else
    timeval curTime;
    gettimeofday(&curTime, NULL);
    ret = curTime.tv_usec;
#endif
    return ret;
}

inline const std::string getCurrentDateTime(const std::string format, bool universalTime, Subseconds subseconds)
{
    std::stringstream stringBuilder;

#if defined( _MSC_VER ) || ( __GNUC__ > 4 )
    auto now = std::chrono::system_clock::now();
    auto inTimeT = std::chrono::system_clock::to_time_t(now);

    if (universalTime)
    {
        stringBuilder << std::put_time(std::gmtime(&inTimeT), format.c_str());
    }
    else
    {
        stringBuilder << std::put_time(std::localtime(&inTimeT), format.c_str());
    }
#else
    // In C++11 std::put_time does this more easily, but in my tests
    // you need minimum GCC 5.1 , so using C library in this case
    time_t rawTime;
    struct tm * timeInfo;
    const std::size_t buffer_size = 32;
    char buffer[buffer_size];

    time(&rawTime);
    if (universalTime)
    {
        timeInfo = gmtime(&rawTime);
    }
    else
    {
        timeInfo = localtime(&rawTime);
    }

    strftime(buffer, buffer_size, format.c_str(), timeInfo);
    stringBuilder << buffer;
#endif

    // Add subseconds
    if ( subseconds == Subseconds::MILLISECONDS )
    {
        // Milliseconds
        stringBuilder << '.' << std::setfill('0') << std::setw(3) << getMilliseconds();
    }
    else if (subseconds == Subseconds::MICROSECONDS)
    {
        // Microseconds
        stringBuilder << '.' << std::setfill('0') << std::setw(6) << getMicroseconds();
    }

    return stringBuilder.str();
}

inline const std::string getUtcDatetime(Subseconds subseconds)
{
    return getCurrentDateTime("%Y%m%d-%H:%M:%S", true, subseconds);
}

inline const Subseconds getSubsecondsFromString(const std::string& input)
{
    if (core::compare(input, "MILLISECONDS"))
    {
        return Subseconds::MILLISECONDS;
    }
    else if (core::compare(input, "MICROSECONDS"))
    {
        return Subseconds::MICROSECONDS;
    }

    return Subseconds::SECONDS;
}

}// namespace

#endif