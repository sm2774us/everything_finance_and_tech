#include "order.h"

#include <sstream>
using namespace std;

#include <core/string_utility.h>

#include "security_manager.h"

namespace order_matcher
{

Order::Order()
{
    m_price = 0;
    m_quantity = 0;
    m_openQuantity = 0;
    m_executedQuantity = 0;
    m_averageExecutedPrice = 0;
    m_lastExecutedPrice = 0;
    m_lastExecutedQuantity = 0;
    m_orderStatus = OrderStatus::ACCEPTED;
    m_lastExecution = OrderStatus::ACCEPTED;
}

Order::Order(size_t orderId, string clientOrderID, size_t securityId, size_t sessionId, OrderSide side, OrderType type, double price, long quantity)
: m_orderID(orderId), m_clientOrderID(clientOrderID), m_securityId(securityId), m_sessionId(sessionId), m_side(side), m_orderType(type), m_price(price), m_quantity(quantity)
{
    m_openQuantity = m_quantity;
    m_executedQuantity = 0;
    m_cancelled = false;
    m_averageExecutedPrice = 0;
    m_lastExecutedPrice = 0;
    m_lastExecutedQuantity = 0;
    m_orderStatus = OrderStatus::ACCEPTED;
    m_lastExecution = OrderStatus::ACCEPTED;
}

void Order::execute(double price, long quantity)
{
    m_averageExecutedPrice = m_averageExecutedPrice * m_executedQuantity;
    m_averageExecutedPrice += price * quantity;
    m_averageExecutedPrice /= (quantity + m_executedQuantity);

    m_openQuantity -= quantity;
    m_executedQuantity += quantity;
    m_lastExecutedPrice = price;
    m_lastExecutedQuantity = quantity;
}

string Order::toString() const
{
    string side = m_side == OrderSide::BUY ? "BUY" : "SELL";
    string ret = core::format("Client session id: %d, Client ID: %s, Symbol: %s, Side: %s", m_sessionId, m_clientOrderID, SecurityManager::getInstance()->getSecurityName(m_securityId), side);
    return ret;
}

ostream& operator<<(ostream& os, Order& order)
{
    os << order.toString() ;
    return os;
}

} // namespace