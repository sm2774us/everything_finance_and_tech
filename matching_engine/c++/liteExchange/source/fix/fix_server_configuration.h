#ifndef _FIX_SERVER_CONFIGURATION_
#define _FIX_SERVER_CONFIGURATION_

#include <string>
#include <core/datetime_utility.h>
#include <core/concurrency/thread_priority.h>

namespace fix
{

#define DEFAULT_TCP_PENDING_CONNECTION_SIZE 32
#define DEFAULT_SOCKET_OPTION_SEND_BUFFER_SIZE 102400
#define DEFAULT_SOCKET_OPTION_RECV_BUFFER_SIZE 102400
#define DEFAULT_POLL_TIMEOUT_MICROSECONDS 1000
#define DEFAULT_POLL_MAX_EVENTS 1024

struct FixServerConfiguration
{
    // TCP
    bool m_tcpDisableNagle;
    bool m_tcpQuickAck;
    int m_tcpPendingConnectionSize;
    int m_tcpSocketOptionSendBufferSize;
    int m_tcpSocketOptionReceiveBufferSize;
    long m_tcpPollTimeoutMicroseconds;
    int m_tcpPollMaxEvents;
    // Reactor thread
    int m_reactorThreadCpuId;
    int m_reactorThreadStackSize;
    core::ThreadPriority m_reactorThreadPriority;
    // FIX
    bool m_fixSequenceNumberValidation;
    std::string m_fixCompId;
    std::string m_fixAddress;
    int m_fixReceiveCacheSize;
    int m_fixPort;
    core::Subseconds m_fixTimePrecision;

    FixServerConfiguration()
    {
        // TCP
        m_tcpDisableNagle = true;
        m_tcpQuickAck = true;
        m_tcpPendingConnectionSize = DEFAULT_TCP_PENDING_CONNECTION_SIZE;
        m_tcpSocketOptionSendBufferSize = DEFAULT_SOCKET_OPTION_SEND_BUFFER_SIZE;
        m_tcpSocketOptionReceiveBufferSize = DEFAULT_SOCKET_OPTION_RECV_BUFFER_SIZE;
        m_tcpPollTimeoutMicroseconds = DEFAULT_POLL_TIMEOUT_MICROSECONDS;
        m_tcpPollMaxEvents = DEFAULT_POLL_MAX_EVENTS;
        // Reactor thread
        m_reactorThreadCpuId = -1;
        m_reactorThreadStackSize = 0;
        m_reactorThreadPriority = core::ThreadPriority::NORMAL;
        // FIX
        m_fixSequenceNumberValidation = true;
        m_fixTimePrecision = core::Subseconds::MICROSECONDS;
        m_fixReceiveCacheSize = 0;
    }
};

}

#endif