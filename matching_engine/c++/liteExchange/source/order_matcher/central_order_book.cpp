#include "central_order_book.h"
#include "security_manager.h"
#include <queue>
using namespace std;

#include <core/compiler/likely.h>
#include <core/stopwatch.h>
#include <core/string_utility.h>
#include <core/logger/logger.h>
using namespace core;

namespace order_matcher
{

CentralOrderBook::CentralOrderBook() : m_isMatchingMultithreaded{ false }
{
    m_orderNumber.store(0);
}

void CentralOrderBook::accept(Visitor<Order>& v)
{
    for (auto& element : m_orderBookDictionary)
    {
        element.second.accept(v);
    }
}

void CentralOrderBook::setSymbols(const std::vector<std::string>& symbols)
{
    int queueID = -1;
    for (auto symbol : symbols)
    {
        auto currentSecurityId = SecurityManager::getInstance()->addSecurity(symbol);
        m_securityIds.push_back(currentSecurityId);
        m_queueIDDictionary.insert(make_pair(currentSecurityId, ++queueID));
    }
}

void CentralOrderBook::initialiseMultithreadedMatching(core::ThreadPoolArguments& args)
{
    LOG_INFO("Central Order Book", core::format("Initialising thread pool, work queue size per thread %d", args.m_workQueueSizePerThread).c_str())

    m_isMatchingMultithreaded = true;

    // Thread names in args are also symbol names
    for (auto itr : m_securityIds)
    {
        auto securityId = itr;
        OrderBook currentOrderBook;
        m_orderBookDictionary.insert(make_pair(securityId, currentOrderBook));
    }

    m_orderBookThreadPool.initialise(args);
}

void CentralOrderBook::initialiseOutgoingMessageQueues(int numberOfThreads, int outgoingMessageProcessorQueueSize)
{
    for (auto i(0); i < numberOfThreads; i++)
    {
        unique_ptr<core::RingBufferSPSCLockFree<OutgoingMessage>> outgoingMessageQueue(new core::RingBufferSPSCLockFree<OutgoingMessage>(outgoingMessageProcessorQueueSize));
        m_outgoingMessages.push_back(std::move(outgoingMessageQueue));
    }
}

bool CentralOrderBook::addOrder(Order& order)
{
    order.setOrderId(++m_orderNumber);

    size_t securityId = order.getSecurityId();
    int queueID = m_queueIDDictionary[securityId];

    // SEND ACCEPTED MESSAGE TO THE CLIENT
    OutgoingMessage message(order, OrderStatus::ACCEPTED);
    m_outgoingMessages[queueID]->push(message);

    if (likely(m_isMatchingMultithreaded))
    {
        // MULTITHREADED MODE : SUBMIT NEW ORDER TASK TO THE THREAD POOL
        core::Task newOrderTask(&CentralOrderBook::taskNewOrder, this, order);
        m_orderBookThreadPool.submitTask(newOrderTask, queueID);
    }
    else
    {
        // SINGLE THREADED MODE : EXECUTE NEW ORDER SYNCHRONOUSLY
        taskNewOrder(order);
    }
    return true;
}

void* CentralOrderBook::taskNewOrder(Order& order)
{
    StopWatch watch;
    watch.start();
    ////////////////////////////////////////////////////////////////
    size_t securityId = order.getSecurityId();
    int queueID = m_queueIDDictionary[securityId];

    m_orderBookDictionary[securityId].insert(order);

    queue<Order> processedOrders;
    m_orderBookDictionary[securityId].processMatching(processedOrders);

    // Append messages to outgoing queue
    // to let the clients know whether orders filled or partially filled
    while (processedOrders.size())
    {
        auto order = processedOrders.front();
        processedOrders.pop();

        // SEND FILLED OR PARTIALLY FILLED MESSAGE TO THE CLIENT
        OutgoingMessage message(order, order.isFilled() ? OrderStatus::FILLED : OrderStatus::PARTIALLY_FILLED);

        m_outgoingMessages[queueID]->push(message);
    }

    ////////////////////////////////////////////////////////////////
    watch.stop();
    return nullptr;
}

void CentralOrderBook::rejectOrder(Order& order, const std::string& message)
{
    LOG_INFO("Central Order Book", core::format("ORDER BEING REJECTED, SESSION ID : %d , CLIENT ORDER ID : %s", order.getSessionId(), order.getClientID()).c_str())
    OutgoingMessage outgoingMessage(order, OrderStatus::REJECTED, message);
    size_t securityId = order.getSecurityId();
    int queueID = m_queueIDDictionary[securityId];
    m_outgoingMessages[queueID]->push(outgoingMessage);
}

void CentralOrderBook::cancelOrder(Order& order, const std::string& origClientOrderID)
{
    auto sessionId = order.getSessionId();
    size_t securityId = order.getSecurityId();

    core::Task cancelOrderTask(&CentralOrderBook::taskCancelOrder, this, order, origClientOrderID);

    if (likely(m_isMatchingMultithreaded))
    {
        int queueID = m_queueIDDictionary[securityId];
        // MULTITHREADED MODE :SUBMIT CANCEL TASK TO THE THREADPOOL
        m_orderBookThreadPool.submitTask(cancelOrderTask, queueID);
    }
    else
    {
        // SINGLE THREADED MODE : CANCEL SYNCHRONOUSLY
        taskCancelOrder(order, origClientOrderID);
    }
}

void* CentralOrderBook::taskCancelOrder(Order& order, const std::string& origClientOrderID)
{
    auto sessionId = order.getSessionId();
    size_t securityId = order.getSecurityId();
    int queueID = m_queueIDDictionary[securityId];
    const OrderSide& side = order.getSide();
    Order* orderBeingCanceled = nullptr;

    if (m_orderBookDictionary[securityId].find(&orderBeingCanceled, origClientOrderID, sessionId, side) == true)
    {
        orderBeingCanceled->cancel();
        OutgoingMessage outgoingMessage(*orderBeingCanceled, OrderStatus::CANCELED, "Order canceled");
        m_outgoingMessages[queueID]->push(outgoingMessage);
        m_orderBookDictionary[securityId].erase(*orderBeingCanceled);
    }
    else
    {
        string message = core::format("Order to cancel could not be found, client session %d, client ID %d", sessionId, origClientOrderID);
        LOG_INFO("Central Order Book", message.c_str())
        rejectOrder(order, message);
    }

    return nullptr;
}

}// namespace