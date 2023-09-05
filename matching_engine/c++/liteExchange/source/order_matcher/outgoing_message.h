#ifndef _OUTGOING_MESSAGE_H_
#define _OUTGOING_MESSAGE_H_

#include <string>
#include "base_message.h"
#include "order.h"
#include <core/pretty_exception.h>

namespace order_matcher
{

class OutgoingMessage : public BaseMessage
{
    public:

        OutgoingMessage()
        {
        }

        OutgoingMessage(Order order, OrderStatus executionType, const std::string& message = "") : BaseMessage(order), m_message(message)
        {
            // Currently do not support replace/amend orders, therefore last execution status and order status will be same
            m_order.setOrderStatus(executionType);
            m_order.setLastExecutionType(executionType);
        }

        std::string toString() const override
        {
            switch (m_order.getLastExecutionType())
            {
            case order_matcher::OrderStatus::ACCEPTED:
                return "ACCEPTED";
                break;
            case order_matcher::OrderStatus::FILLED:
                return "FILLED";
                break;
            case order_matcher::OrderStatus::PARTIALLY_FILLED:
                return "PARTIALLY_FIELD";
                break;
            case order_matcher::OrderStatus::CANCELED:
                return "CANCELED";
                break;
            case order_matcher::OrderStatus::REJECTED:
                return "REJECTED";
                break;
            default:
                THROW_PRETTY_INVALID_ARG_EXCEPTION(std::string("Non supported outgoing message type"))
                    break;
            }
        }

        bool hasMessage() const { return m_message.length() != 0 ; }

    private:

        std::string m_message;
};

} // namespace

#endif