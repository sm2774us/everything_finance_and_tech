#ifndef _LOG_LEVELS_
#define _LOG_LEVELS_

#include <string>
#include <core/string_utility.h>
#include <core/pretty_exception.h>

namespace core
{

enum class LogLevel { LEVEL_FATAL, LEVEL_ERROR, LEVEL_WARNING, LEVEL_INFO, LEVEL_DEBUG };

inline void logLevelToString(LogLevel level, std::string& outputString)
{
    switch (level)
    {
        case LogLevel::LEVEL_FATAL:
            outputString = "FATAL";
            break;

        case LogLevel::LEVEL_INFO:
            outputString = "INFO";
            break;

        case LogLevel::LEVEL_WARNING:
            outputString = "WARNING";
            break;

        case LogLevel::LEVEL_ERROR:
            outputString = "ERROR";
            break;

        case LogLevel::LEVEL_DEBUG:
            outputString = "DEBUG";
            break;

        default:
            THROW_PRETTY_RUNTIME_EXCEPTION("Invalid log level type");
    }
}

inline LogLevel logLevelFromString(const std::string& logLevelString)
{
    LogLevel level = LogLevel::LEVEL_ERROR;

    if (core::compare(logLevelString, "INFO"))
    {
        level = LogLevel::LEVEL_INFO;
    }
    else if (core::compare(logLevelString, "WARNING"))
    {
        level = LogLevel::LEVEL_WARNING;
    }
    else if (core::compare(logLevelString, "ERROR"))
    {
        level = LogLevel::LEVEL_ERROR;
    }
    else if (core::compare(logLevelString, "DEBUG"))
    {
        level = LogLevel::LEVEL_DEBUG;
    }
    else if (core::compare(logLevelString, "FATAL"))
    {
        level = LogLevel::LEVEL_FATAL;
    }
    else
    {
        THROW_PRETTY_RUNTIME_EXCEPTION("Invalid log level type");
    }

    return level;
}

}

#endif