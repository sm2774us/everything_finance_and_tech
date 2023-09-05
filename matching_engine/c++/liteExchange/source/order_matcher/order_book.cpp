#include "order_book.h"
#include <core/pretty_exception.h>
#include <core/string_utility.h>
#include <core/logger/logger.h>
#include <utility>
using namespace std;
using namespace core;

namespace order_matcher
{

void OrderBook::accept(Visitor<Order>& v)
{
    for (auto& element : m_bidOrders)
    {
        v.visit(element.second);
    }

    for (auto& element : m_askOrders)
    {
        v.visit(element.second);
    }
}

void OrderBook::insert(const Order& order)
{
    if (order.getSide() == OrderSide::BUY)
    {
        m_bidOrders.insert( make_pair(order.getPrice(), order));
    }
    else
    {
        m_askOrders.insert( make_pair(order.getPrice(), order));
    }
}

bool OrderBook::find(Order** order, const string& clientID, size_t sessionId, OrderSide side)
{
    if (side == OrderSide::BUY)
    {
        for (auto i = m_bidOrders.begin(); i != m_bidOrders.end(); ++i)
        {
            if (i->second.getSessionId() == sessionId && i->second.getClientID() == clientID)
            {
                *order = &(i->second);
                return true;
            }
        }
    }
    else if (side == OrderSide::SELL)
    {
        for (auto i = m_askOrders.begin(); i != m_askOrders.end(); ++i)
        {
            if (i->second.getSessionId() == sessionId  && i->second.getClientID() == clientID)
            {
                *order = &(i->second);
                return true;
            }
        }
    }

    return false;
}

void OrderBook::erase(const Order& order)
{
    auto clientId = order.getClientID();
    auto sessionId = order.getSessionId();

    if (order.getSide() == OrderSide::BUY)
    {
        for (auto i = m_bidOrders.begin(); i != m_bidOrders.end(); ++i)
        {
            if (i->second.getClientID() == clientId && i->second.getSessionId() == sessionId)
            {
                m_bidOrders.erase(i);
                return;
            }
        }
    }
    else if (order.getSide() == OrderSide::SELL)
    {
        for (auto i = m_askOrders.begin(); i != m_askOrders.end(); ++i)
        {
            if (i->second.getClientID() == clientId && i->second.getSessionId() == sessionId)
            {
                m_askOrders.erase(i);
                return;
            }
        }
    }
}

void OrderBook::matchTwoOrders(Order& bid, Order& ask)
{
    double price = ask.getPrice();
    long quantity = 0;

    if (bid.getOpenQuantity() > ask.getOpenQuantity())
    {
        quantity = ask.getOpenQuantity();
    }
    else
    {
        quantity = bid.getOpenQuantity();
    }

    bid.execute(price, quantity);
    ask.execute(price, quantity);
}


// Will return true if any order processed
// Otherwise return false
bool OrderBook::processMatching(queue<Order>& processedOrders)
{
    while (true)
    {
        if (!m_bidOrders.size() || !m_askOrders.size())
        {
            return processedOrders.size() != 0;
        }

        auto iBid = m_bidOrders.begin();
        auto iAsk = m_askOrders.begin();

        if (iBid->second.getPrice() >= iAsk->second.getPrice())
        {
            Order& bid = iBid->second;
            Order& ask = iAsk->second;

            matchTwoOrders(bid, ask);

            processedOrders.push(bid);
            processedOrders.push(ask);

            if (bid.isFilled())
            {
                LOG_INFO("Order book", core::format("ORDER FULLY FILLED, SESSION ID : %d , CLIENT ORDER ID : %s", bid.getSessionId(), bid.getClientID()).c_str())
                m_bidOrders.erase(iBid);
            }

            if (ask.isFilled())
            {
                LOG_INFO("Order book", core::format("ORDER FULLY FILLED, SESSION ID : %d , CLIENT ORDER ID : %s", ask.getSessionId(), ask.getClientID()).c_str())
                m_askOrders.erase(iAsk);
            }
        }
        else
        {
            return processedOrders.size() != 0;
        }
    }
}

} // namespace