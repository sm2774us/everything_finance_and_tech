#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <cstddef>
#include <string>

#include <core/concurrency/actor.h>
#include <core/concurrency/thread_wait_strategy.h>
#include <core/concurrency/queue_mpsc.hpp>

#include <core/design_patterns/singleton_dclp.hpp>

#include <core/logger/logger_arguments.h>
#include <core/logger/log_levels.h>
#include <core/logger/log_entry.hpp>
#include <core/logger/memory_mapped_file_backend.hpp>

namespace core
{

#define STRINGIFY_DETAIL(x) #x
#define STRINGIFY(x) STRINGIFY_DETAIL(x)

#define LOG_DEBUG(SENDER, MESSAGE) (core::Logger::getInstance()->log(core::LogLevel::LEVEL_DEBUG,(SENDER),(MESSAGE)));
#define LOG_INFO(SENDER, MESSAGE) (core::Logger::getInstance()->log(core::LogLevel::LEVEL_INFO,(SENDER),(MESSAGE)));
#define LOG_WARNING(SENDER, MESSAGE) (core::Logger::getInstance()->log(core::LogLevel::LEVEL_WARNING,(SENDER),(MESSAGE)));
#define LOG_ERROR(SENDER, MESSAGE) (core::Logger::getInstance()->log(core::LogLevel::LEVEL_ERROR,(SENDER),(MESSAGE)));
#define LOG_FATAL(SENDER, MESSAGE) (core::Logger::getInstance()->log(core::LogLevel::LEVEL_FATAL,(SENDER),(MESSAGE)));

using LogBuffer = std::unique_ptr< core::QueueMPSC<LogEntry> >;

class Logger : public core::Actor, public SingletonDCLP<Logger>, public SleepWaitStrategy
{
    public :
        Logger() : Actor("LoggerThread"), m_backendEnabled{ false }, m_copyToStdout{ false }
        {}
        ~Logger() { shutdown(); }

        void initialise(const LoggerArguments&);
        void log(const LogLevel level, const std::string& sender, const std::string& message);
        void* run() override;

    private:
        void pushLogToLogBuffer(const LogEntry& log);
        bool m_backendEnabled;
        bool m_copyToStdout;
        // Move ctor deletion
        Logger(Logger&& other) = delete;
        // Move assignment operator deletion
        Logger& operator=(Logger&& other) = delete;

        LogBuffer m_buffer;
        MemoryMappedFileBackend m_backend;
        std::size_t m_writePeriodInMicroseconds = DEFAULT_LOGGER_WRITE_PERIOD_MILLISECONDS*1000;
        int m_logLevel = 0;
};

}//namespace
#endif