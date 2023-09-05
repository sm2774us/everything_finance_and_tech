#ifndef _ORDER_H_
#define _ORDER_H_

#include <string>
#include <cstddef>

namespace order_matcher
{

enum class OrderSide { NON_SUPPORTED, BUY, SELL };
enum class OrderType { NON_SUPPORTED, LIMIT  };
enum class OrderStatus { ACCEPTED, PARTIALLY_FILLED, FILLED, DONE_FOR_DAY, CANCELED, REPLACED, PENDING_CANCEL, STOPPED, REJECTED };

class Order
{
    public:

        Order();
        Order(std::size_t orderId, std::string clientOrderID, std::size_t securityId, std::size_t sessionId, OrderSide side, OrderType type, double price, long quantity);
        void execute(double price, long quantity);
        void cancel() { m_cancelled = true; }
        std::string toString() const;

        bool isFilled() const { return m_executedQuantity == m_quantity; }
        bool isPartiallyFilled() const { return (m_executedQuantity > 0 && m_executedQuantity < m_quantity) ? true : false; }
        bool isCancelled() const { return m_cancelled; }

        void setOrderId(std::size_t id) { m_orderID = id; }
        void setQuantity(long quantity) { m_quantity = quantity;  m_openQuantity = quantity; }
        void setPrice(double price) { m_price = price; }
        void setClientOrderId(const std::string& clientOrderId) { m_clientOrderID = clientOrderId; }
        void setSessionId(std::size_t sessionId) { m_sessionId = sessionId; }
        void setOrderType(OrderType type) { m_orderType = type; }
        void setOrderSide(OrderSide side) { m_side = side; }
        void setSecurityId(std::size_t securityId) { m_securityId = securityId; }
        void setOrderStatus(OrderStatus status) { m_orderStatus = status; }
        void setLastExecutionType(OrderStatus execType) { m_lastExecution = execType; }

        std::size_t getOrderId() const { return m_orderID; }
        long getQuantity() const { return m_quantity; }
        long getOpenQuantity() const { return m_openQuantity; }
        long getExecutedQuantity() const { return m_executedQuantity; }
        double getPrice() const { return m_price; }
        const std::string& getClientID() const { return m_clientOrderID; }
        std::size_t getSecurityId() const { return m_securityId; }
        std::size_t getSessionId() const { return m_sessionId; }
        OrderSide getSide() const { return m_side; }
        OrderType getOrderType() const { return m_orderType; }
        double getAverageExecutedPrice() const { return m_averageExecutedPrice; }
        double getLastExecutedPrice() const { return m_lastExecutedPrice; }
        long getLastExecutedQuantity() const { return m_lastExecutedQuantity; }
        OrderStatus getOrderStatus() const { return m_orderStatus; }
        OrderStatus getLastExecutionType() const { return m_lastExecution; }

        friend std::ostream& operator<<(std::ostream& os, Order& entry);

    private:
        std::size_t m_orderID;
        std::string m_clientOrderID;
        std::size_t m_securityId;
        std::size_t m_sessionId;
        OrderSide m_side;
        OrderType m_orderType;
        double m_price;
        long m_quantity;

        long m_openQuantity;
        long m_executedQuantity;
        bool m_cancelled;
        double m_averageExecutedPrice;
        double m_lastExecutedPrice;
        long m_lastExecutedQuantity;

        OrderStatus m_orderStatus;
        OrderStatus m_lastExecution;
};

} // namespace

#endif