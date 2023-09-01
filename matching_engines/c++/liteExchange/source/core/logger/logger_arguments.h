#ifndef _LOGGER_ARGUMENTS_H_
#define _LOGGER_ARGUMENTS_H_

#include <cstddef>
#include <string>
#include <core/logger/log_levels.h>

namespace core
{

#define DEFAULT_LOGGER_RING_BUFFER_SIZE 819200
#define DEFAULT_LOGGER_ROTATION_SIZE_IN_BYTES 10240000
#define DEFAULT_LOGGER_WRITE_PERIOD_MILLISECONDS 1000

struct LoggerArguments
{
    std::size_t m_bufferSize;
    std::size_t m_rotationSizeInBytes;
    std::size_t m_writePeriodInMilliSeconds;
    LogLevel m_logLevel;
    bool m_copyToStdout;
    std::string m_memoryMappedFileName;

    LoggerArguments() : m_bufferSize{ DEFAULT_LOGGER_RING_BUFFER_SIZE },
    m_rotationSizeInBytes{ DEFAULT_LOGGER_ROTATION_SIZE_IN_BYTES }, m_writePeriodInMilliSeconds{ DEFAULT_LOGGER_WRITE_PERIOD_MILLISECONDS },
    m_logLevel{ LogLevel::LEVEL_ERROR }, m_copyToStdout{ false }
    {}
};


}//namespace
#endif