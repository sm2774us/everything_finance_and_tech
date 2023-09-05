#include "fix_session.h"
#include "fix_constants.h"
#include "fix_parser.h"
#include "fix_sequence_store.h"
#include <core/compiler/likely.h>
#include <memory>
#include <mutex>
using namespace std;

namespace fix
{

string FixSession::COMPID = "";

FixSession::FixSession() : m_fixVersion{ FixConstants::FixVersion::FIX_VERSION_NONE }, m_outgoingSequenceNumber{ 1 }, m_incomingSequenceNumber{ 1 },
m_heartBeatInterval{ DEFAULT_HEARTBEAT_INTERVAL }, m_encryptionMethod{ FixConstants::FIX_ENCRYPTION_NONE }
, m_tcpConnection{ nullptr }, m_sessionLock{ nullptr }, m_sessionSendLock{nullptr}, m_receiveCache{ nullptr }, m_isUsingExternalTCPConnectionManager{ false }
{
    m_timePrecision = core::Subseconds::MICROSECONDS;
    m_lastReceivedTime = 0;
    m_lastSentTime = 0;
}

FixSession::FixSession(const FixSession& other)
{
    m_outgoingSequenceNumber = other.m_outgoingSequenceNumber;
    m_incomingSequenceNumber = other.m_incomingSequenceNumber;
    m_heartBeatInterval = other.m_heartBeatInterval;
    m_encryptionMethod = other.m_encryptionMethod;
    m_tcpConnection = other.m_tcpConnection;
    m_sessionLock = other.m_sessionLock;
    m_sessionSendLock = other.m_sessionSendLock;
    m_receiveCache = other.m_receiveCache;
    m_targetCompId = other.m_targetCompId;
    m_fixVersion = other.m_fixVersion;
    m_timePrecision = other.m_timePrecision;
    m_lastReceivedTime = other.m_lastReceivedTime;
    m_lastSentTime = other.m_lastSentTime;
}

FixSession::~FixSession()
{
    if (m_sessionLock)
    {
        delete m_sessionLock;
    }

    if (m_sessionSendLock)
    {
        delete m_sessionSendLock;
    }

    if (m_receiveCache)
    {

        delete m_receiveCache;
    }

    if (!m_isUsingExternalTCPConnectionManager)
    {
        close();
    }
}

void FixSession::initialise(core::TCPConnection* connection, core::Subseconds timePrecision, size_t receiveCacheSize)
{
    m_tcpConnection = connection;
    m_timePrecision = timePrecision;
    m_sessionLock = new core::SpinLock;
    m_sessionSendLock = new core::SpinLock;
    m_receiveCache = new FixReceiveCache;

    if (receiveCacheSize > 0)
    {
        m_receiveCache->initialise(receiveCacheSize);
    }
}

void FixSession::close()
{
    if (m_tcpConnection)
    {
        m_tcpConnection->close();
    }
}

size_t FixSession::getSecondsSinceLastReceive()
{
    size_t ret = static_cast<size_t> (difftime(time(0), m_lastReceivedTime));
    return ret;
}

size_t FixSession::getSecondsSinceLastSend()
{
    size_t ret = static_cast<size_t> (difftime(time(0), m_lastSentTime));
    return ret;
}

void FixSession::restoreSequenceNumbersFromStore()
{
    FixSequenceStore::loadFromSequenceStore(FixSession::COMPID, m_targetCompId, m_outgoingSequenceNumber, m_incomingSequenceNumber);
}

void FixSession::saveSequenceNumbersToStore()
{
    FixSequenceStore::updateSequenceStore(FixSession::COMPID, m_targetCompId, m_outgoingSequenceNumber, m_incomingSequenceNumber);
}

void FixSession::getBaseMessage(int messageType, FixMessage& message)
{
    message.setFixVersion(m_fixVersion);
    message.setMessageType(messageType);
    message.setSenderCompId(FixSession::COMPID);
    message.setTargetCompId(m_targetCompId);
    // Sequence number and sending time will be added during sending
}

void FixSession::getLogonMessage(FixMessage& message)
{
    getBaseMessage(FixConstants::MessageType::LOGON, message);
    message.setTag(FixConstants::FIX_TAG_HEARBEAT_INTERVAL, m_heartBeatInterval);
    message.setTag(FixConstants::FIX_TAG_ENCRYPT_METHOD, m_encryptionMethod);
}

void FixSession::getAdminRejectionMessage(FixMessage& message)
{
    getBaseMessage(FixConstants::MessageType::ADMIN_REJECT, message);
    message.setTag(FixConstants::FIX_TAG_REF_SEQ_NUM, message.getTagValue(FixConstants::FIX_TAG_SEQUENCE_NUMBER));
}

void FixSession::getLogoffMessage(FixMessage& message)
{
    getBaseMessage(FixConstants::MessageType::LOGOFF, message);
}

void FixSession::getHeartBeatMessage(FixMessage& message)
{
    getBaseMessage(FixConstants::MessageType::HEARTBEAT, message);
}

void FixSession::getExecutionReportMessage(FixMessage& message)
{
    getBaseMessage(FixConstants::MessageType::EXECUTION_REPORT, message);
}

void FixSession::getUserLogonResponseMessage(FixMessage& message)
{
    getBaseMessage(FixConstants::MessageType::USER_RESPONSE, message);
}

void FixSession::handleLogonMessage(const FixMessage& message)
{
    const string& senderCompId = message.getSenderCompId();
    setTargetCompId(senderCompId);

    restoreSequenceNumbersFromStore();

    int heartbeatInverval = message.getTagValueAsInt(FixConstants::FIX_TAG_HEARBEAT_INTERVAL);
    setHeartBeatInterval(heartbeatInverval);
    setFixVersion(message.getFixVersion());

    int encryptionMethod = message.getTagValueAsInt(FixConstants::FIX_TAG_ENCRYPT_METHOD);
    setEncryptionMethod(encryptionMethod);

    FixMessage logonResponse;
    getLogonMessage(logonResponse);
    send(logonResponse);
}

void FixSession::handleAdminMessage(const FixMessage& message)
{
    auto messageType = message.getMessageType();

    if (messageType == FixConstants::MessageType::LOGOFF)
    {
        FixMessage logoffMessage;
        getLogoffMessage(logoffMessage);
        send(logoffMessage);
    }
    else if (messageType == FixConstants::MessageType::HEARTBEAT)
    {
        FixMessage heartbeatResponse;
        getHeartBeatMessage(heartbeatResponse);
        send(heartbeatResponse);
    }
    else if (messageType == FixConstants::MessageType::TEST_REQUEST)
    {
        const string& testRequestId = message.getTagValue(FixConstants::FIX_TAG_TEST_REQUEST_ID);
        FixMessage testRequestResponse;
        getHeartBeatMessage(testRequestResponse);
        testRequestResponse.setTag(FixConstants::FIX_TAG_TEST_REQUEST_ID, testRequestId);
        send(testRequestResponse);
    }
    else if (messageType == FixConstants::MessageType::USER_LOGON)
    {
        const string& userRequestId = message.getTagValue(FixConstants::FIX_TAG_USER_REQUEST_ID);
        const string& userName = message.getTagValue(FixConstants::FIX_TAG_USERNAME);
        FixMessage userLogonResponse;
        getUserLogonResponseMessage(userLogonResponse);
        userLogonResponse.setTag(FixConstants::FIX_TAG_USER_REQUEST_ID, userRequestId);
        userLogonResponse.setTag(FixConstants::FIX_TAG_USERNAME, userName);
        send(userLogonResponse);
    }
}

void FixSession::receiveWithCaching(vector<FixMessage>* messages)
{
    int receivedSize{ 0 };
    auto remainingBytes = m_receiveCache->getRemainingBufferSize();
    receivedSize = m_tcpConnection->receive(m_receiveCache->getBuffer(), remainingBytes);
    if (receivedSize > 0)
    {
        m_receiveCache->incrementSize(receivedSize);
        updateLastReceivedTime();
        m_receiveCache->parse(messages);
    }
}

int FixSession::receive(FixMessage& message)
{
    const size_t trailerSize{ 7 };// 7 is because of 10=081@
    const size_t initialBufferSize{ 20 };    // Length of 8=FIX.4.2@9=7000@35= so we always get 35=

    const size_t stackBufferSize{ 1024 };
    char stackBuffer[stackBufferSize];

    int receivedSize{ 0 };

    receivedSize = m_tcpConnection->receive(stackBuffer, initialBufferSize);

    if (receivedSize <= 0)
    {
        return receivedSize;
    }

    size_t fixBodyLength{ 0 };
    size_t receivedBodyStartIndex{ 0 };

    for (int i{ 0 }; i < receivedSize; ++i)
    {
        // Find out body length
        if (stackBuffer[i] == '9' && stackBuffer[i + 1] == FixConstants::FIX_EQUALS)
        {
            string bodyLength;

            for (int j{ i + 2 }; j < receivedSize; j++)
            {
                char currentChar = stackBuffer[j];

                if (currentChar == FixConstants::FIX_DELIMITER)
                {
                    fixBodyLength = std::stoi(bodyLength);
                    i = j;
                    break;
                }
                else
                {
                    bodyLength.push_back(stackBuffer[j]);
                }
            }
        }

        if (stackBuffer[i] == '3' && stackBuffer[i + 1] == '5')
        {
            // Find out received body start index
            receivedBodyStartIndex = i;
            break;
        }
    }

    size_t receivedBodyLength = receivedSize - receivedBodyStartIndex;
    size_t remainingBytes = fixBodyLength - receivedBodyLength + trailerSize;

    size_t stackBufferActualLength = receivedSize;
    size_t totalLength = stackBufferActualLength;

    if (unlikely(remainingBytes + initialBufferSize > stackBufferSize))
    {
        // FROM BOTH STACK MEMORY AND HEAP MEMORY
        unique_ptr<char> heapBuffer(new char[remainingBytes]);

        receivedSize = m_tcpConnection->receive(heapBuffer.get(), remainingBytes);

        if (receivedSize <= 0)
        {
            return receivedSize;
        }

        totalLength += receivedSize;
        FixParser::parseFromMultipleBuffers(message, stackBuffer, stackBufferActualLength, heapBuffer.get(), receivedSize);
    }
    else
    {
        // ALL FROM STACK MEMORY
        receivedSize = m_tcpConnection->receive(stackBuffer + stackBufferActualLength, remainingBytes);

        if (receivedSize <= 0)
        {
            return receivedSize;
        }

        totalLength += receivedSize;
        FixParser::parseFromBuffer(message, stackBuffer, totalLength);
    }

    updateLastReceivedTime();
    return totalLength;
}

bool FixSession::validateSequenceNumber(int incomingSequenceNumber)
{
    if (getIncomingSequenceNumber() + 1 != incomingSequenceNumber)
    {
        return false;
    }

    return true;
}

bool FixSession::validateSequenceNumber(const FixMessage& message)
{
    auto actualIncomingSequenceNumber = message.getSequenceNumber();
    return validateSequenceNumber(actualIncomingSequenceNumber);
}

bool FixSession::validateTargetCompid(const FixMessage& message)
{
    const string& targetCompId = message.getTargetCompId();

    if (!core::compare(targetCompId, FixSession::COMPID))
    {
        return false;
    }

    return true;
}

bool FixSession::validateRequiredTags(const FixMessage& message, int& missingTag)
{
    auto verifyTag = [&](int tag) -> bool
    {
        if (message.hasTag(tag) == false)
        {
            missingTag = tag;
            return false;
        }
        return true;
    };

    if (message.getFixVersion() == FixConstants::FixVersion::FIX_VERSION_NONE) return false;
    if (verifyTag(FixConstants::FIX_TAG_BODY_LENGTH) == false) return false;
    if (message.getMessageType() == FixConstants::MessageType::MESSAGE_TYPE_NONE) return false;
    if (verifyTag(FixConstants::FIX_TAG_BODY_CHECKSUM) == false) return false;
    if (message.getSequenceNumber() == 0) return false;
    if (message.getSenderCompId().length()  == 0) return false;
    if (message.getTargetCompId().length() == 0) return false;

    auto messageType = message.getMessageType();

    switch (messageType)
    {
        case FixConstants::MessageType::LOGON:
            if (verifyTag(FixConstants::FIX_TAG_HEARBEAT_INTERVAL) == false) return false;
            if (verifyTag(FixConstants::FIX_TAG_ENCRYPT_METHOD) == false) return false;
            break;

        case FixConstants::MessageType::NEW_ORDER:
            if (message.getSendingTime().length() == 0) return false;
            if (verifyTag(FixConstants::FIX_TAG_SYMBOL) == false) return false;
            if (verifyTag(FixConstants::FIX_TAG_CLIENT_ORDER_ID) == false) return false;
            if (verifyTag(FixConstants::FIX_TAG_ORDER_QUANTITY) == false) return false;
            if (verifyTag(FixConstants::FIX_TAG_ORDER_PRICE) == false) return false;
            if (verifyTag(FixConstants::FIX_TAG_ORDER_SIDE) == false) return false;
            if (verifyTag(FixConstants::FIX_TAG_ORDER_TYPE) == false) return false;
            if (verifyTag(FixConstants::FIX_TAG_TRANSACTION_TIME) == false) return false;
            break;

        case FixConstants::MessageType::CANCEL:
            if (message.getSendingTime().length() == 0) return false;
            if (verifyTag(FixConstants::FIX_TAG_CLIENT_ORDER_ID) == false) return false;
            if (verifyTag(FixConstants::FIX_TAG_ORIG_CLIENT_ORDER_ID) == false) return false;
            break;
        default:
            break;
    }
    return true;
}

int FixSession::send(FixMessage& message)
{
    int ret{ -1 };

    // FixVersion, MessageType, SenderCompId,TargetCompId are added by getBaseMessage

    bool isAdminMessage = message.isAdminMessage();
    string messageAsString;
    auto currentUTCDateTime = core::getUtcDatetime(m_timePrecision);

    {
        // Critical section
        std::lock_guard < core::SpinLock > sendLock(*m_sessionSendLock);

        message.setSequenceNumber(m_outgoingSequenceNumber);
        message.setSendingTime(currentUTCDateTime);

        if (isAdminMessage == false)
        {
            message.setTag(FixConstants::FIX_TAG_TRANSACTION_TIME, currentUTCDateTime);
        }

        message.toString(messageAsString);

        ret = m_tcpConnection->send(messageAsString);
        m_outgoingSequenceNumber++;

        m_lastSentTime = time(0);
    }

    return ret;
}

}