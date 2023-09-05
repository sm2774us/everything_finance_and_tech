#include <core/string_utility.h>
#include "fix_server_reactor.h"
#include "fix_constants.h"
using namespace std;

namespace fix
{

vector<unique_ptr<FixSession>> FixServerReactor::Sessions;
vector<bool> FixServerReactor::SessionFlags;

FixServerReactor::FixServerReactor() : TCPServerReactor(), m_receiveCacheSize{0}, m_sequenceNumberValidation{true}
{
}

void FixServerReactor::addSession(FixSession* newSession)
{
    auto currentSize = Sessions.size();
    int nonUserPeerIndex{ -1 };

    for (size_t i{ 0 }; i < currentSize; i++)
    {
        if (SessionFlags[i] == false)
        {
            nonUserPeerIndex = i;
            break;
        }
    }

    if (nonUserPeerIndex == -1)
    {
        // No empty slot , create new
        SessionFlags.push_back(true);
        Sessions.emplace_back(newSession);
    }
    else
    {
        // Use an existing peer slot
        Sessions[nonUserPeerIndex].reset(newSession);
        SessionFlags[nonUserPeerIndex] = true;
    }
}

void FixServerReactor::removeSession(std::size_t sessionIndex)
{
    Sessions[sessionIndex]->close();
    SessionFlags[sessionIndex] = false;
}

void FixServerReactor::initialise(const FixServerConfiguration& configuration)
{
    // Socket options
    m_socket.setSocketOption(core::SOCKET_OPTION::TCP_DISABLE_NAGLE, configuration.m_tcpDisableNagle == true ? 1 : 0);
    m_socket.setSocketOption(core::SOCKET_OPTION::TCP_ENABLE_QUICKACK, configuration.m_tcpQuickAck == true ? 1 : 0);
    m_socket.setSocketOption(core::SOCKET_OPTION::RECEIVE_BUFFER_SIZE, configuration.m_tcpSocketOptionReceiveBufferSize);
    m_socket.setSocketOption(core::SOCKET_OPTION::SEND_BUFFER_SIZE, configuration.m_tcpSocketOptionSendBufferSize);
    m_socket.setPendingConnectionsQueueSize(configuration.m_tcpPendingConnectionSize);

    // Polling options
    setPollTimeout(configuration.m_tcpPollTimeoutMicroseconds);
    setMaxPollEvents(configuration.m_tcpPollMaxEvents);

    // Thread options
    setReactorThreadCpuId(configuration.m_reactorThreadCpuId);
    setReactorThreadPriority(configuration.m_reactorThreadPriority);
    setReactorThreadStackSize(configuration.m_reactorThreadStackSize);

    // Fix options
    m_address = configuration.m_fixAddress;
    m_port = configuration.m_fixPort;
    FixSession::setCompId(configuration.m_fixCompId);
    m_timePrecision = configuration.m_fixTimePrecision;
    m_receiveCacheSize = configuration.m_fixReceiveCacheSize;
    m_sequenceNumberValidation = configuration.m_fixSequenceNumberValidation;
}

void FixServerReactor::start()
{
    TCPServerReactor::start(m_address, m_port);
}

FixSession* FixServerReactor::getSession(size_t index)
{
    if (FixServerReactor::SessionFlags[index] == false)
    {
        return nullptr;
    }

    return (FixServerReactor::Sessions)[index].get();
}

void FixServerReactor::stop()
{
    TCPServerReactor::stop();
    for (auto& session : FixServerReactor::Sessions)
    {
        if (session->connected())
        {
            session->saveSequenceNumbersToStore();
        }
    }
}

void FixServerReactor::onClientConnected(size_t peerIndex)
{
    TCPServerReactor::onClientConnected(peerIndex);

    FixSession newSession;

    FixServerReactor::addSession(new FixSession());

    auto session = getSession(peerIndex);
    session->setUsesExternalTcpConnectionManager(true);
    session->initialise(getPeerSocket(peerIndex), m_timePrecision, m_receiveCacheSize);
}

void FixServerReactor::onClientDisconnected(size_t peerIndex)
{
    auto session = getSession(peerIndex);

    onFixLogoff(session);

    session->lock();

    session->saveSequenceNumbersToStore();

    FixServerReactor::removeSession(peerIndex);

    TCPServerReactor::onClientDisconnected(peerIndex);

    session->unlock();
}

void FixServerReactor::processMessage(FixMessage* incomingMessage, FixSession* session, size_t peerIndex)
{
    auto messageType = incomingMessage->getMessageType();
    int missingTag = 0;

    // Validations done in fix server level for all sessions
    if (FixSession::validateRequiredTags(*incomingMessage, missingTag) == false)
    {
        // ADMIN LEVEL REJECTION FOR MISSING TAGS
        FixMessage adminLevelRejectionMessage;
        session->getAdminRejectionMessage(adminLevelRejectionMessage);
        adminLevelRejectionMessage.setTag(FixConstants::FIX_TAG_FREE_TEXT, "Required tag missing");
        adminLevelRejectionMessage.setTag(FixConstants::FIX_TAG_REF_TAG, missingTag);
        adminLevelRejectionMessage.setTag(FixConstants::FIX_TAG_REF_MSG_TYPE, FixConstants::FIX_MESSAGE_TYPE_STRINGS[incomingMessage->getMessageType()] );
        adminLevelRejectionMessage.setTag(FixConstants::FIX_TAG_REJECT_REASON, 1);
        session->send(adminLevelRejectionMessage);
        return;
    }

    // Logon messages need to be handled before seq number validation
    if (messageType == FixConstants::MessageType::LOGON)
    {
        if (!FixSession::validateTargetCompid(*incomingMessage))
        {
            onFixError(core::format("Invalid target comp id , expected : %s , actual : %s", FixSession::getCompId(), incomingMessage->getTargetCompId().c_str()), session);
            session->close();
            onClientDisconnected(peerIndex);
            return;
        }
        session->handleLogonMessage(*incomingMessage);
        onFixLogon(session);
    }

    // Sequence number validation
    auto actualIncomingSequenceNumber = incomingMessage->getSequenceNumber();

    if (m_sequenceNumberValidation == true)
    {
        if (!session->validateSequenceNumber(actualIncomingSequenceNumber))
        {
            // Currently sequence number resetting / gap filling not supported
            onFixError(core::format("Invalid sequence number received , expected : %d , actual : %d", session->getIncomingSequenceNumber()+1, actualIncomingSequenceNumber), session);
            session->close();
            onClientDisconnected(peerIndex);
            return;
        }
    }

    session->setIncomingSequenceNumber(actualIncomingSequenceNumber);

    if (incomingMessage->isAdminMessage())
    {
        session->handleAdminMessage(*incomingMessage);

        auto messageType = incomingMessage->getMessageType();

        if (messageType == FixConstants::MessageType::LOGOFF)
        {
            onClientDisconnected(peerIndex);
        }
        else if (messageType == FixConstants::MessageType::USER_LOGON)
        {
            auto trader = incomingMessage->getTagValue(FixConstants::FIX_TAG_USERNAME);
            onTraderLogon(session, trader);
        }
    }
    else
    {
        onFixMessage(incomingMessage, peerIndex);
    }
}

void FixServerReactor::checkErrors(int recvSize, FixSession* session, size_t peerIndex)
{
    auto error = core::Socket::getCurrentThreadLastSocketError();

    if (session->getTCPConnection()->isConnectionLost(error, recvSize))
    {
        onClientDisconnected(peerIndex);
    }
    else
    {
        onUnhandledSocketError(error, recvSize);
    }
}

void* FixServerReactor::handleClient(size_t peerIndex)
{
    auto session = getSession(peerIndex);

    // Sometimes epoll/select might return data before connection request
    // If data arrives before connection request, session and its tcp connection won`t be ready
    if (session == nullptr)
    {
        return nullptr;
    }

    if (!session->getTCPConnection())
    {
        return nullptr;
    }

    if (m_receiveCacheSize == 0)
    {
        // RECEIVE MESSAGES ONE BY ONE
        FixMessage incomingMessage;

        auto receivedSize = session->receive(incomingMessage);

        if (receivedSize <= 0)
        {
            checkErrors(receivedSize, session, peerIndex);
            return nullptr;
        }

        processMessage(&incomingMessage, session, peerIndex);
    }
    else
    {
        // RECEIVE MULTIPLE MESSAGES AT ONCE
        vector<FixMessage> messages;
        session->receiveWithCaching(&messages);
        auto messageNumber = messages.size();

        if (messageNumber == 0)
        {
            checkErrors(0, session, peerIndex);
            return nullptr;
        }

        for (size_t i{ 0 }; i < messageNumber; ++i)
        {
            processMessage(&((messages)[i]), session, peerIndex);
        }
    }

    return nullptr;
}

}