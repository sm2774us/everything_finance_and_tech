#ifndef _SERVER_CONFIGURATION_H_
#define _SERVER_CONFIGURATION_H_

#include <vector>
#include <string>

#include <server/server_constants.h>

#include <core/configuration.h>

#include <core/concurrency/thread_pool_arguments.h>
#include <core/concurrency/thread_priority.h>

#include <core/logger/logger_arguments.h>

#include <core/pretty_exception.h>

#include <fix/fix_server_configuration.h>

class ServerConfiguration
{
    public :

        ServerConfiguration()
        : m_outgoingMessageQueueSizePerThread{ 0 }, m_isMatchingMultithreaded{ false }
        {
        }

        void load(const std::string& configurationFile)
        {
            core::Configuration configuration;
            core::Configuration::loadFromFile(configurationFile, configuration);

            m_processPriority = configuration.getStringValue(server_constants::CONFIGURATION_FILE_PROCESS_PRIORITY, server_constants::CONFIGURATION_DEFAULT_PROCESS_PRIORITY);

            // ORDER ENTRY MODE
            m_orderEntryMode = configuration.getStringValue(server_constants::CONFIGURATION_FILE_ORDER_ENTRY_MODE, "FIX");

            // Offline order entry configuration
            m_offlineOrderEntryFile = configuration.getStringValue(server_constants::CONFIGURATION_FILE_OFFLINE_ORDER_ENTRY_FILE);
            m_offlineOrderEntryOutputFile = configuration.getStringValue(server_constants::CONFIGURATION_FILE_OFFLINE_ORDER_ENTRY_OUTPUT_FILE);

            // Fix server configuration
            m_fixServerConfiguration.m_tcpDisableNagle = configuration.getBoolValue(server_constants::CONFIGURATION_FILE_TCP_DISABLE_NAGLE, true);
            m_fixServerConfiguration.m_tcpQuickAck = configuration.getBoolValue(server_constants::CONFIGURATION_FILE_TCP_QUICK_ACK, true);
            m_fixServerConfiguration.m_tcpPendingConnectionSize = configuration.getIntValue(server_constants::CONFIGURATION_FILE_TCP_PENDING_CONNECTION_SIZE, DEFAULT_TCP_PENDING_CONNECTION_SIZE);
            m_fixServerConfiguration.m_tcpSocketOptionReceiveBufferSize = configuration.getIntValue(server_constants::CONFIGURATION_FILE_TCP_SOCKET_OPTION_RECEIVE_BUFFER_SIZE, DEFAULT_SOCKET_OPTION_RECV_BUFFER_SIZE);
            m_fixServerConfiguration.m_tcpSocketOptionSendBufferSize = configuration.getIntValue(server_constants::CONFIGURATION_FILE_TCP_SOCKET_OPTION_SEND_BUFFER_SIZE, DEFAULT_SOCKET_OPTION_SEND_BUFFER_SIZE);
            m_fixServerConfiguration.m_tcpPollTimeoutMicroseconds = configuration.getIntValue(server_constants::CONFIGURATION_FILE_TCP_POLL_TIMEOUT_MICROSECONDS, DEFAULT_POLL_TIMEOUT_MICROSECONDS);
            m_fixServerConfiguration.m_tcpPollMaxEvents = configuration.getIntValue(server_constants::CONFIGURATION_FILE_TCP_POLL_MAX_EVENTS, DEFAULT_POLL_MAX_EVENTS);

            m_fixServerConfiguration.m_reactorThreadPriority = core::getThreadPriorityFromString(configuration.getStringValue(server_constants::CONFIGURATION_FILE_REACTOR_THREAD_PRIORITY));
            m_fixServerConfiguration.m_reactorThreadCpuId = configuration.getIntValue(server_constants::CONFIGURATION_FILE_REACTOR_THREAD_CPU_ID, -1);
            m_fixServerConfiguration.m_reactorThreadStackSize = configuration.getIntValue(server_constants::CONFIGURATION_FILE_REACTOR_THREAD_STACK_SIZE, 0);

            m_fixServerConfiguration.m_fixCompId = configuration.getStringValue(server_constants::CONFIGURATION_FILE_FIX_SERVER_COMP_ID);
            m_fixServerConfiguration.m_fixAddress = configuration.getStringValue(server_constants::CONFIGURATION_FILE_FIX_SERVER_ADDRESS);
            m_fixServerConfiguration.m_fixPort = configuration.getIntValue(server_constants::CONFIGURATION_FILE_FIX_SERVER_PORT);
            m_fixServerConfiguration.m_fixTimePrecision = core::getSubsecondsFromString(configuration.getStringValue(server_constants::CONFIGURATION_FILE_FIX_SERVER_TIME_PRECISION, "MICROSECONDS"));
            m_fixServerConfiguration.m_fixReceiveCacheSize = configuration.getIntValue(server_constants::CONFIGURATION_FILE_FIX_SERVER_RECEIVE_CACHE_SIZE, 0);
            m_fixServerConfiguration.m_fixSequenceNumberValidation = configuration.getBoolValue(server_constants::CONFIGURATION_FILE_FIX_SERVER_SEQUENCE_NUMBER_VALIDATION, true);

            // Get logger configuration
            m_loggerConfiguration.m_copyToStdout = configuration.getBoolValue(server_constants::CONFIGURATION_FILE_LOGGER_COPY_TO_STDOUT, false);
            m_loggerConfiguration.m_bufferSize = configuration.getIntValue(server_constants::CONFIGURATION_FILE_LOGGER_BUFFER_SIZE, DEFAULT_LOGGER_RING_BUFFER_SIZE);
            m_loggerConfiguration.m_writePeriodInMilliSeconds = configuration.getIntValue(server_constants::CONFIGURATION_FILE_LOGGER_WRITE_PERIOD_MILLISECONDS, DEFAULT_LOGGER_WRITE_PERIOD_MILLISECONDS);
            m_loggerConfiguration.m_rotationSizeInBytes = configuration.getIntValue(server_constants::CONFIGURATION_FILE_LOGGER_ROTATION_SIZE_IN_BYTES, DEFAULT_LOGGER_ROTATION_SIZE_IN_BYTES);
            m_loggerConfiguration.m_memoryMappedFileName = configuration.getStringValue(server_constants::CONFIGURATION_FILE_LOGGER_MEMORY_MAPPED_FILE, "");
            m_loggerConfiguration.m_logLevel = core::logLevelFromString( configuration.getStringValue(server_constants::CONFIGURATION_FILE_LOGGER_LOG_LEVEL, "") );

            // Get symbol configuration
            m_symbols = configuration.getArray(server_constants::CONFIGURATION_FILE_SYMBOL_ARRAY);

            if (m_symbols.size() == 0)
            {
                THROW_PRETTY_RUNTIME_EXCEPTION("No symbol found in the ini file");
            }

            // Get matching multithreading mode
            m_isMatchingMultithreaded = configuration.getBoolValue(server_constants::CONFIGURATION_FILE_CENTRAL_ORDER_BOOK_MULTITHREADED_ORDER_MATCHING, server_constants::CONFIGURATION_DEFAULT_CENTRAL_ORDER_BOOK_MULTITHREADED_ORDER_MATCHING_ENGINE);

            // Get multithreading configuration
            if (m_isMatchingMultithreaded)
            {
                m_threadPoolArguments.m_pinThreadsToCores = configuration.getBoolValue(server_constants::CONFIGURATION_FILE_CENTRAL_ORDER_BOOK_PIN_THREADS_TO_CORES, server_constants::CONFIGURATION_DEFAULT_CENTRAL_ORDER_BOOK_PIN_THREADS_TO_CORES);
                m_threadPoolArguments.m_hyperThreading = configuration.getBoolValue(server_constants::CONFIGURATION_FILE_CENTRAL_ORDER_BOOK_HYPER_THREADING, server_constants::CONFIGURATION_DEFAULT_CENTRAL_ORDER_BOOK_HYPER_THREADING);
                m_threadPoolArguments.m_workQueueSizePerThread = configuration.getIntValue(server_constants::CONFIGURATION_FILE_CENTRAL_ORDER_BOOK_WORK_QUEUE_SIZE_PER_THREAD, server_constants::CONFIGURATION_DEFAULT_CENTRAL_ORDER_BOOK_WORK_QUEUE_SIZE_PER_THREAD);
                m_threadPoolArguments.m_threadStackSize = configuration.getIntValue(server_constants::CONFIGURATION_FILE_CENTRAL_ORDER_BOOK_THREAD_STACK_SIZE, server_constants::CONFIGURATION_DEFAULT_CENTRAL_ORDER_BOOK_THREAD_STACK_SIZE);
                m_threadPoolArguments.m_threadPriority = core::getThreadPriorityFromString(configuration.getStringValue(server_constants::CONFIGURATION_FILE_CENTRAL_ORDER_BOOK_THREAD_PRIORITY, server_constants::CONFIGURATION_DEFAULT_CENTRAL_ORDER_BOOK_THREAD_PRIORITY));
            }

            // Get outgoing message processor configuration
            m_outgoingMessageQueueSizePerThread = configuration.getIntValue(server_constants::CONFIGURATION_FILE_OUTGOING_MESSAGE_QUEUE_SIZE_PER_THREAD, server_constants::CONFIGURATION_DEFAULT_OUTGOING_MESSAGE_QUEUE_SIZE_PER_THREAD);
            m_outgoingMessageThreadCpuId = configuration.getIntValue(server_constants::CONFIGURATION_FILE_OUTGOING_MESSAGE_QUEUE_THREAD_CPU_ID, -1);
            outgoingMessageThreadStackSize = configuration.getIntValue(server_constants::CONFIGURATION_FILE_OUTGOING_MESSAGE_QUEUE_THREAD_STACK_SIZE, 0);
            m_outgoingMessageThreadPriority = core::getThreadPriorityFromString(configuration.getStringValue(server_constants::CONFIGURATION_FILE_OUTGOING_MESSAGE_QUEUE_THREAD_PRIORITY, "NORMAL"));
        }

        const std::string getOrderEntryMode() const { return m_orderEntryMode; }
        const std::string getProcessPriority() const { return m_processPriority;  }
        bool getMatchingMultithreadingMode() const { return m_isMatchingMultithreaded; }
        core::ThreadPoolArguments getThreadPoolArguments() const { return m_threadPoolArguments; }
        std::vector<std::string> getSymbols() const { return m_symbols;  }
        const std::string getOfflineOrderEntryFile() const { return m_offlineOrderEntryFile; }
        const std::string getOfflineOrderEntryOutputFile() const { return m_offlineOrderEntryOutputFile; }
        core::LoggerArguments& getLoggerConfiguration() { return m_loggerConfiguration; }
        const fix::FixServerConfiguration getFixServerConfiguration() const { return m_fixServerConfiguration;  }

        int getOutgoingMessageQueueSizePerThread()const { return m_outgoingMessageQueueSizePerThread; }
        int getOutgoingMessageThreadCPUId()const { return m_outgoingMessageThreadCpuId; }
        int getOutgoingMessageThreadStackSize()const { return outgoingMessageThreadStackSize; }
        core::ThreadPriority getOutgoingMessageThreadPriority()const { return m_outgoingMessageThreadPriority; }


    private :
        int m_outgoingMessageQueueSizePerThread;
        int m_outgoingMessageThreadCpuId;
        int outgoingMessageThreadStackSize;
        core::ThreadPriority m_outgoingMessageThreadPriority;
        std::string m_orderEntryMode;
        std::string m_processPriority;
        std::string m_offlineOrderEntryFile;
        std::string m_offlineOrderEntryOutputFile;
        bool m_isMatchingMultithreaded;
        core::ThreadPoolArguments m_threadPoolArguments;
        std::vector<std::string> m_symbols;
        core::LoggerArguments m_loggerConfiguration;
        fix::FixServerConfiguration m_fixServerConfiguration;
};


#endif