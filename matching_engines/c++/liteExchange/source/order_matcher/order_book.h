#ifndef _ORDER_BOOK_H_
#define _ORDER_BOOK_H_

#include "order.h"

#include <cstddef>
#include <map>
#include <queue>
#include <string>
#include <functional>
#include <memory>

#include <core/design_patterns/visitor.hpp>

namespace order_matcher
{

class OrderBook : public core::Visitable<Order>
{
    public :
        OrderBook() = default;

        void accept(core::Visitor<Order>& v) override;
        void insert(const Order& order);
        bool find(Order** order, const std::string& clientID, std::size_t sessionId, OrderSide side);
        void erase(const Order& order);
        bool processMatching( std::queue<Order>& processedOrders );
        bool isEmpty() const { return (m_bidOrders.size() == 0) && (m_askOrders.size() == 0); }

    private:

        // Bid orders , place the greatest on top
        std::multimap < double, Order, std::greater < double > > m_bidOrders;
        // Ask orders , place the lowest on top
        std::multimap < double, Order, std::less < double > > m_askOrders;

        void matchTwoOrders(Order& bid, Order& ask);
};

using OrderBookPtr = std::unique_ptr<order_matcher::OrderBook>;

} //namespace

#endif