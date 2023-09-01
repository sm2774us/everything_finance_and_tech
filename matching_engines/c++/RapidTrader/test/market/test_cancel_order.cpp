#include "market_test_fixture.h"

/**
 * Tests cancelling a portion of an order's quantity such that it is not removed from the book.
 */
TEST_F(MarketTest, CancelOrderShouldWork1)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitAskOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    uint64_t cancel_quantity = 100;

    market.cancelOrder(symbol_id, id1, cancel_quantity);

    checkOrderAdded(id1);
    checkOrderUpdated(id1, 0, 0, quantity1 - cancel_quantity);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests cancelling a portion of an order's quantity such that it is removed from the book.
 */
TEST_F(MarketTest, CancelOrderShouldWork2)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    uint64_t cancel_quantity = 200;

    market.cancelOrder(symbol_id, id1, cancel_quantity);

    checkOrderAdded(id1);
    checkOrderUpdated(id1, 0, 0, 0);
    checkOrderDeleted(id1, 0, 0, 0);
    ASSERT_TRUE(market_debugger.empty());
}