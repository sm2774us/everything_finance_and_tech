#ifndef _INCOMING_MESSAGE_H_
#define _INCOMING_MESSAGE_H_

#include <string>
#include "base_message.h"
#include "order.h"
#include <core/pretty_exception.h>

namespace order_matcher
{

enum class IncomingMessageType { NEW_ORDER, CANCEL_ORDER };

class IncomingMessage : public BaseMessage
{
    public:

        IncomingMessage()
        {
        }

        IncomingMessage(Order order, IncomingMessageType type, const std::string& origClientOrderID="")
        : BaseMessage(order), m_originalOrderID(origClientOrderID), m_type(type)
        {
        }

        IncomingMessageType& getType() { return m_type; }

        std::string& getOrigClientOrderID() { return m_originalOrderID; }

        std::string toString() const override
        {
            switch (m_type)
            {
                case order_matcher::IncomingMessageType::NEW_ORDER:
                    return "NEW_ORDER";
                    break;
                case order_matcher::IncomingMessageType::CANCEL_ORDER:
                    return "CANCEL_ORDER";
                    break;
                default:
                    THROW_PRETTY_INVALID_ARG_EXCEPTION(std::string("Invalid incoming message type"))
                    break;
            }
        }

    private:
        std::string m_originalOrderID; // Only applies to cancel messages
        IncomingMessageType m_type;
};

} // namespace

#endif