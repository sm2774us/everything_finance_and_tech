#include "server_outgoing_message_processor.h"

#include <cassert>

#include <core/logger/logger.h>

#include <core/file_utility.h>
#include <core/compiler/likely.h>
#include <core/datetime_utility.h>
#include <core/string_utility.h>

#include <fix/fix_constants.h>
#include <fix/fix_session.h>
#include <fix/fix_server_reactor.h>

#include <order_matcher/security_manager.h>

#include <server/fix_message_converter.h>
#include <server/server_constants.h>

using namespace core;
using namespace fix;
using namespace order_matcher;
using namespace std;

OutgoingMessageProcessor::OutgoingMessageProcessor() : Actor("OutgoingWorker"), m_messageQueue{ nullptr }, m_numberOfQueues{ 0 }, m_execID{ 0 }, m_offlineMode{ false }
// We can`t have more than 16 characters in Linux for a pthread name ,that is why compacted the thread name...
{
    m_outgoingMessage.reset(new order_matcher::OutgoingMessage);
}

void OutgoingMessageProcessor::enableOfflineMode(const string& offlineModeOutputFile)
{
    m_offlineMode = true;
    m_offlineModeOutputFile = offlineModeOutputFile;
}

void OutgoingMessageProcessor::setMessageQueue(OutgoingMessageQueue* queue, int numberOfQueues)
{
    assert(queue != nullptr);
    m_messageQueue = queue;
    m_numberOfQueues = numberOfQueues;
}

void OutgoingMessageProcessor::rejectOrder(size_t sessionId, FixMessage* message, const string& rejectionMessage)
{
    // BUSINESS LEVEL REJECTS
    FixSession* session{ nullptr };
    FixMessage rejectMessage;

    if (unlikely(m_offlineMode))
    {
        rejectMessage.setMessageType(FixConstants::MessageType::BUSINESS_REJECT);
    }
    else
    {
        session = FixServerReactor::getSession(sessionId);

        if (session == nullptr)
        {
            LOG_ERROR("Outgoing message processor", core::format("Session(%d) not available, cannot send reject message : %d", sessionId, rejectionMessage))
            return;
        }

        session->lock();

        session->getBaseMessage(FixConstants::MessageType::BUSINESS_REJECT, rejectMessage);
    }

    // Currently supporting only new orders and cancels, as not supporting amending orders both order status and exec types will be rejected
    rejectMessage.setTag(FixConstants::FIX_TAG_ORDER_STATUS, FixConstants::FIX_ORDER_STATUS_REJECTED);
    rejectMessage.setTag(FixConstants::FIX_TAG_EXEC_TYPE, FixConstants::FIX_ORDER_STATUS_REJECTED);
    rejectMessage.setTag(FixConstants::FIX_TAG_FREE_TEXT, rejectionMessage);

    if (unlikely(m_offlineMode))
    {
        rejectMessage.setSendingAndTransactionTimeAsCurrent(core::Subseconds::MICROSECONDS);
        std::string rejectMessageContent;
        rejectMessage.toString(rejectMessageContent);
        core::appendTextToFile(m_offlineModeOutputFile, rejectMessageContent + '\n', true);
    }
    else
    {
        session->send(rejectMessage);
        session->unlock();
    }
}

int OutgoingMessageProcessor::generateExecID()
{
    ++m_execID;
    return m_execID;
}

void* OutgoingMessageProcessor::run()
{
    LOG_INFO("Outgoing message processor", "Outgoing message processor thread starting")

    // Let`s wait until message queue initialisation

    while (true)
    {
        if (isFinishing() == true)
        {
            break;
        }

        if (m_messageQueue == nullptr)
        {
            core::Thread::sleep(server_constants::SERVER_THREAD_SLEEP_DURATION_MICROSECONDS);
        }
        else
        {
            break;
        }
    }

    while (true)
    {
        if (isFinishing() == true)
        {
            break;
        }

        for (auto i(0); i < m_numberOfQueues; i++)
        {
            if ((*m_messageQueue)[i]->tryPop(m_outgoingMessage.get()) == true)
            {
                const Order& order = m_outgoingMessage->getOrder();

                FixSession* session{ nullptr };
                fix::FixMessage executionReport;

                if (unlikely(m_offlineMode))
                {
                    executionReport.setMessageType(FixConstants::MessageType::EXECUTION_REPORT);
                }
                else
                {
                    session = fix::FixServerReactor::getSession(order.getSessionId());

                    if (session == nullptr)
                    {
                        LOG_ERROR("Outgoing message processor",core::format("Session not available, cannot send execution report : SessionId(%d) ClientOrderId(%s)", order.getSessionId(), order.getClientID()))
                        continue;
                    }

                    session->lock();

                    session->getExecutionReportMessage(executionReport);
                }

                int orderStatus = FixMessageConverter::convertOrderStatusToFixOrderStatus(order.getOrderStatus());
                int executionType = FixMessageConverter::convertOrderStatusToFixOrderStatus(order.getLastExecutionType());

                if (order.getSide() == order_matcher::OrderSide::BUY)
                {
                    executionReport.setTag(fix::FixConstants::FIX_TAG_ORDER_SIDE, fix::FixConstants::FIX_ORDER_SIDE_BUY);
                }
                else
                {
                    executionReport.setTag(fix::FixConstants::FIX_TAG_ORDER_SIDE, fix::FixConstants::FIX_ORDER_SIDE_SELL);
                }

                executionReport.setTag(fix::FixConstants::FIX_TAG_CLIENT_ORDER_ID, order.getClientID());
                executionReport.setTag(fix::FixConstants::FIX_TAG_ORDER_ID, order.getClientID());
                executionReport.setTag(fix::FixConstants::FIX_TAG_EXEC_ID, generateExecID());
                executionReport.setTag(fix::FixConstants::FIX_TAG_ORDER_STATUS, orderStatus);
                executionReport.setTag(fix::FixConstants::FIX_TAG_EXEC_TYPE, executionType);
                executionReport.setTag(fix::FixConstants::FIX_TAG_ORDER_QUANTITY, order.getQuantity());
                executionReport.setTag(fix::FixConstants::FIX_TAG_LEAVES_QTY, order.getOpenQuantity());
                executionReport.setTag(fix::FixConstants::FIX_TAG_CUMULATIVE_QUANTITY, order.getExecutedQuantity());
                executionReport.setTag(fix::FixConstants::FIX_TAG_AVERAGE_PRICE, order.getAverageExecutedPrice());
                executionReport.setTag(fix::FixConstants::FIX_TAG_SYMBOL, order_matcher::SecurityManager::getInstance()->getSecurityName(order.getSecurityId()));

                if (orderStatus == fix::FixConstants::FIX_ORDER_STATUS_FILLED || orderStatus == fix::FixConstants::FIX_ORDER_STATUS_PARTIALLY_FILLED)
                {
                    executionReport.setTag(fix::FixConstants::FIX_TAG_LAST_QUANTITY, order.getLastExecutedQuantity());
                    executionReport.setTag(fix::FixConstants::FIX_TAG_LAST_PRICE, order.getLastExecutedPrice());
                }

                if (executionReport.getFixVersion() == fix::FixConstants::FIX_4_2)
                {
                    executionReport.setTag(fix::FixConstants::FIX_TAG_EXEC_TRANSTYPE, 0);
                }

                if (unlikely(m_offlineMode))
                {
                    // OFFLINE ORDER ENTRY MODE
                    executionReport.setSendingAndTransactionTimeAsCurrent(core::Subseconds::MICROSECONDS);
                    std::string executionReportContent;
                    executionReport.toString(executionReportContent);
                    core::appendTextToFile(m_offlineModeOutputFile, executionReportContent + '\n', true);
                }
                else
                {
                    // FIX MODE
                    auto result = session->send(executionReport);

                    string logMessage = "";

                    if (orderStatus == fix::FixConstants::FIX_ORDER_STATUS_FILLED || orderStatus == fix::FixConstants::FIX_ORDER_STATUS_PARTIALLY_FILLED)
                    {
                        logMessage = "SENDING FILL EXECUTION REPORT : ";
                    }
                    else
                    {
                        logMessage = "SENDING EXECUTION REPORT : ";
                    }

                    logMessage += core::format("TargetCompid(%s) ClientOrderId(%s) ExecType(%d) SocketResult(%d)", session->getTargetCompId().c_str(), order.getClientID().c_str(), executionType, result);
                    LOG_INFO("Outgoing message processor", logMessage.c_str())

                    // TODO : Currently losing an execution report in case of a possible disconnection
                    // If send result is a failure need to resend this to a client in case of reconnection

                    session->unlock();
                }
            }
        }

    }// while

    LOG_INFO("Outgoing message processor", "Outgoing message processor thread exiting")

    return nullptr;
}