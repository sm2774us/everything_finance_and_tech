#include <server/server_fix.h>

#include <algorithm>
#include <ctype.h>
#include <cstddef>
#include <exception>

#include <core/compiler/unused.h>
#include <core/concurrency/thread.h>
#include <core/concurrency/thread_priority.h>

#include <order_matcher/order.h>
#include <order_matcher/incoming_message.h>
#include <order_matcher/security_manager.h>
using namespace order_matcher;

#include <core/file_utility.h>
#include <core/pretty_exception.h>
#include <core/logger/logger.h>

#include <fix/fix_constants.h>

#include "fix_message_converter.h"
#include "server_outgoing_message_processor.h"

using namespace std;
using namespace fix;

ServerFix::ServerFix(const ServerConfiguration& serverConfiguration)
: ServerBase(serverConfiguration), FixServerReactor()
{
    FixServerReactor::initialise(serverConfiguration.getFixServerConfiguration());
    //CLI
    m_commandLineInterface.setParentCentralOrderbook(&m_centralOrderBook);
}

ServerFix::~ServerFix()
{
    m_outgoingMessageProcessor.shutdown();
}

void ServerFix::run()
{
    start();
    LOG_INFO("FIX Engine", "Acceptor started")

    m_commandLineInterface.start();

    while (true)
    {
        // Exit if interface returns false , it means we are exiting
        if (m_commandLineInterface.isFinishing())
        {
            break;
        }

        for (auto& thread : core::Thread::THREADS)
        {
            auto threadException = thread->getException();
            if (threadException)
            {
                LOG_ERROR("FIX Server", "Unhandled exception in thread : " + thread->getName())
                std::rethrow_exception(threadException);
            }
        }
    }

    stop();
    LOG_INFO("FIX Engine", "Acceptor stopped")
}

void ServerFix::onUnhandledSocketError(int errorCode, int eventResult)
{
    LOG_ERROR("FIX Server", core::format( "Socket error :%d, socket event result :%d ", errorCode, eventResult))
}

void ServerFix::onFixError(const std::string& fixErrorMessage, FixSession* session)
{
    LOG_ERROR("FIX Server", core::format("Fix error for %s : %s", session->getTargetCompId(), fixErrorMessage))
}

void ServerFix::onFixLogon(FixSession* session)
{
    LOG_INFO("FIX Server", "New logon , comp id : " + session->getTargetCompId())
}

void ServerFix::onFixLogoff(FixSession* session)
{
    LOG_INFO("FIX Server", "Logoff , comp id : " + session->getTargetCompId())
}

void ServerFix::onTraderLogon(FixSession* session, const string& trader)
{
    UNUSED(session);
    LOG_INFO("FIX Server", "Trader logon  : " + trader)
}

void ServerFix::onFixMessage(FixMessage* message, size_t sessionId)
{
    auto messageType = message->getMessageType();

    if (messageType == FixConstants::MessageType::NEW_ORDER)
    {
        onNewOrder(message, sessionId);
    }
    else if (messageType == FixConstants::MessageType::CANCEL)
    {
        onCancelOrder(message, sessionId);
    }
    else
    {
        m_outgoingMessageProcessor.rejectOrder(sessionId, message, "Only new and cancel orders accepted");
    }
}

void ServerFix::onNewOrder(FixMessage* message, size_t sessionId)
{
    Order order;
    order.setSessionId(sessionId);

    FixMessageConverter::convertNewOrder(*message, order);

    if (message->getTagValueAsInt(fix::FixConstants::FIX_TAG_ORDER_TYPE) != fix::FixConstants::FIX_ORDER_TYPE_LIMIT)
    {
        m_outgoingMessageProcessor.rejectOrder(sessionId, message, "Non supported order type");
        return;
    }

    if (order_matcher::SecurityManager::getInstance()->isSecuritySupported(order.getSecurityId()) == false)
    {
        m_outgoingMessageProcessor.rejectOrder(sessionId, message, "Non supported symbol");
        return;
    }

    LOG_INFO("FIX Server", core::format("NEW ORDER RECEIVED, SESSION ID : %d, CLIENT ORDER ID : %s", sessionId, order.getClientID()).c_str())

    m_centralOrderBook.addOrder(order);
}

void ServerFix::onCancelOrder(FixMessage* message, size_t sessionId)
{
    Order order;
    order.setSessionId(sessionId);

    FixMessageConverter::convertCancelOrder(*message, order);

    LOG_INFO("FIX Server", core::format("CANCEL ORDER RECEIVED : %s , SESSION ID : %d", order.getClientID(), sessionId).c_str())

    m_centralOrderBook.cancelOrder(order, message->getTagValue(FixConstants::FIX_TAG_ORIG_CLIENT_ORDER_ID));
}