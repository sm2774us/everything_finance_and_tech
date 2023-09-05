#ifndef _FIX_MESSAGE_CONVERTER_H_
#define _FIX_MESSAGE_CONVERTER_H_

#include <string>
#include <type_traits>

#include <core/logger/logger.h>

#include <fix/fix_message.h>
#include <fix/fix_constants.h>

#include <order_matcher/security_manager.h>
#include <order_matcher/order.h>

class FixMessageConverter
{
    public:

        static void convertCommonOrder(const fix::FixMessage& message, order_matcher::Order& order)
        {
            const std::string& symbol = message.getTagValue(fix::FixConstants::FIX_TAG_SYMBOL);
            auto securityId = order_matcher::SecurityManager::getInstance()->getSecurityId(symbol);
            order.setSecurityId(securityId);
            order.setClientOrderId(message.getTagValue(fix::FixConstants::FIX_TAG_CLIENT_ORDER_ID));
        }

        static void convertNewOrder(const fix::FixMessage& message, order_matcher::Order& order)
        {
            convertCommonOrder(message, order);

            auto orderType = message.getTagValueAsInt(fix::FixConstants::FIX_TAG_ORDER_TYPE);

            if (orderType == fix::FixConstants::FIX_ORDER_TYPE_LIMIT)
            {
                order.setOrderType(order_matcher::OrderType::LIMIT);
            }
            else
            {
                LOG_WARNING("FixMessageConverter", "Only limit orders are supported")
            }

            auto side = message.getTagValueAsInt(fix::FixConstants::FIX_TAG_ORDER_SIDE);

            if (side == fix::FixConstants::FIX_ORDER_SIDE_BUY)
            {
                order.setOrderSide(order_matcher::OrderSide::BUY);
            }
            else if (side == fix::FixConstants::FIX_ORDER_SIDE_SELL)
            {
                order.setOrderSide(order_matcher::OrderSide::SELL);
            }

            order.setQuantity(message.getTagValueAsLong(fix::FixConstants::FIX_TAG_ORDER_QUANTITY));
            order.setPrice(message.getTagValueAsLong(fix::FixConstants::FIX_TAG_ORDER_PRICE));

        }

        static void convertCancelOrder(const fix::FixMessage& message, order_matcher::Order& order)
        {
            convertCommonOrder(message, order);
        }

        static int convertOrderStatusToFixOrderStatus(order_matcher::OrderStatus status)
        {
            int ret = static_cast<std::underlying_type<order_matcher::OrderStatus>::type>(status);
            return ret;
        }

};

#endif