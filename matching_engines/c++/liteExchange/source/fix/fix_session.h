#ifndef _FIX_SESSION_
#define _FIX_SESSION_

#include <string>
#include <ctime>

#include <core/network/tcp_connection.h>
#include <core/datetime_utility.h>
#include <core/concurrency/spinlock.hpp>
#include <memory>
#include <vector>
#include "fix_message.h"
#include "fix_receive_cache.h"

namespace fix
{

#define DEFAULT_HEARTBEAT_INTERVAL 30

class FixSession
{
    public:
        FixSession();
        FixSession(const FixSession& other);
        ~FixSession();

        void initialise(core::TCPConnection* connection, core::Subseconds timePrecision, std::size_t receiveCacheSize);

        int receive(FixMessage&);    // NON THREAD SAFE
        void receiveWithCaching(std::vector<FixMessage>*);
        int send(FixMessage&);        // THREAD SAFE

        void restoreSequenceNumbersFromStore();
        void saveSequenceNumbersToStore();

        void setUsesExternalTcpConnectionManager(bool value) { m_isUsingExternalTCPConnectionManager = value; }
        bool connected()const { return m_tcpConnection != nullptr; }
        void close();

        std::size_t getSecondsSinceLastReceive();
        std::size_t getSecondsSinceLastSend();

        void lock() { m_sessionLock->lock(); }
        void tryLock() { m_sessionLock->tryLock(); }
        void unlock() { m_sessionLock->unlock(); }

        void getBaseMessage(int messageType, FixMessage& fixMessage);
        void getLogonMessage(FixMessage&);
        void getLogoffMessage(FixMessage&);
        void getHeartBeatMessage(FixMessage&);
        void getExecutionReportMessage(FixMessage&);
        void getUserLogonResponseMessage(FixMessage&);
        void getAdminRejectionMessage(FixMessage&);
        int getIncomingSequenceNumber() const { return m_incomingSequenceNumber; }

        void handleLogonMessage(const FixMessage&);
        void handleAdminMessage(const FixMessage&);

        core::TCPConnection* getTCPConnection() { return m_tcpConnection; }
        int getHeartBeatInterval() const { return m_heartBeatInterval; }
        const std::string& getTargetCompId() const { return m_targetCompId; }
        core::Subseconds getTimePrecision() const { return m_timePrecision; }

        void setIncomingSequenceNumber(int number) { m_incomingSequenceNumber = number;  }
        void setOutgoingSequenceNumber(int number) { m_outgoingSequenceNumber = number; }
        void setHeartBeatInterval(int number) { m_heartBeatInterval = number; }
        void setEncryptionMethod(int number) { m_encryptionMethod = number; }
        void setTargetCompId(const std::string& targetCompId) { m_targetCompId = targetCompId; }
        void setFixVersion(int fixVersion) { m_fixVersion = fixVersion; }
        void setTimePrecision(core::Subseconds timePrecision) { m_timePrecision = timePrecision; }
        static void setCompId(const std::string& compId) { FixSession::COMPID = compId; }
        static const std::string& getCompId() { return FixSession::COMPID; }

        bool validateSequenceNumber(const FixMessage&);
        bool validateSequenceNumber(int);
        static bool validateTargetCompid(const FixMessage&);
        static bool validateRequiredTags(const FixMessage&, int& misingTag);

    private:
        int m_fixVersion;
        int m_outgoingSequenceNumber;
        int m_incomingSequenceNumber;
        int m_heartBeatInterval;
        int m_encryptionMethod;
        core::TCPConnection* m_tcpConnection;
        core::SpinLock* m_sessionLock;
        core::SpinLock* m_sessionSendLock;
        FixReceiveCache* m_receiveCache;
        std::string m_targetCompId;
        core::Subseconds  m_timePrecision;
        time_t m_lastReceivedTime;
        time_t m_lastSentTime;
        bool m_isUsingExternalTCPConnectionManager;

        // SENDER COMPID SHARED BY ALL SESSIONS
        static std::string COMPID;

        void updateLastReceivedTime() { m_lastReceivedTime = time(0); }
};

}

#endif