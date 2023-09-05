#include "market_test_fixture.h"

/**
 * Tests replacing an order such that it does not result in matching.
 */
TEST_F(MarketTest, ReplaceOrderShouldWork1)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 1000;
    uint64_t price1 = 1500;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    uint64_t new_order_id = 2;
    uint64_t new_order_price = 1200;
    market.replaceOrder(symbol_id, id1, new_order_id, new_order_price);

    checkOrderAdded(id1);
    checkOrderAdded(new_order_id);
    checkOrderDeleted(id1, 0, 0, quantity1);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests replacing an order such that it does result in matching.
 */
TEST_F(MarketTest, ReplaceOrderShouldWork2)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 100;
    uint64_t price1 = 1500;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 1000;
    uint64_t price2 = 1200;
    uint64_t id2 = 2;
    Order order2 = Order::limitBidOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    OrderTimeInForce tof3 = OrderTimeInForce::GTC;
    uint64_t quantity3 = 500;
    uint64_t price3 = 2000;
    uint64_t id3 = 3;
    Order order3 = Order::limitAskOrder(id3, symbol_id, price3, quantity3, tof3);
    market.addOrder(order3);

    uint64_t new_order_id = 4;
    uint64_t new_order_price = 900;
    market.replaceOrder(symbol_id, id3, new_order_id, new_order_price);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkOrderAdded(id3);
    checkOrderAdded(new_order_id);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkExecutedOrder(new_order_id, price1, quantity1, quantity3 - quantity1);
    checkExecutedOrder(id2, price2, quantity3 - quantity1, quantity2 - (quantity3 - quantity1));
    checkExecutedOrder(new_order_id, price2, quantity3 - quantity1, 0);
    checkOrderDeleted(id3, 0, 0, quantity3);
    checkOrderDeleted(id1, price1, quantity1, 0);
    checkOrderDeleted(new_order_id, price2, quantity3 - quantity1, 0);
    ASSERT_TRUE(market_debugger.empty());
}