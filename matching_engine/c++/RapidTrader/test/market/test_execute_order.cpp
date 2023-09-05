#include "market_test_fixture.h"
/**
 * Tests Executing an order with a provided quantity but not a price.
 */
TEST_F(MarketTest, ExecuteOrderShouldWork1)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitAskOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    uint64_t executed_quantity = 100;
    market.executeOrder(symbol_id, id1, executed_quantity);

    checkOrderAdded(id1);
    checkExecutedOrder(id1, price1, executed_quantity, quantity1 - executed_quantity);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests Executing an order with a provided quantity and price.
 */
TEST_F(MarketTest, ExecuteOrderShouldWork2)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitAskOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    uint64_t executed_quantity = 100;
    uint32_t executed_price = 400;
    market.executeOrder(symbol_id, id1, executed_quantity, executed_price);

    checkOrderAdded(id1);
    checkExecutedOrder(id1, executed_price, executed_quantity, quantity1 - executed_quantity);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests Executing an order with a provided quantity such that it must be deleted.
 */
TEST_F(MarketTest, ExecuteOrderShouldWork3)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitAskOrder(id1, symbol_id, price1, quantity1, tof1); // Add the order.
    market.addOrder(order1);

    uint64_t executed_quantity = 200;
    market.executeOrder(symbol_id, id1, executed_quantity);

    checkOrderAdded(id1);
    checkExecutedOrder(id1, price1, executed_quantity, 0);
    checkOrderDeleted(id1, price1, executed_quantity, 0);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests Executing an order with a provided quantity and price such that it must be deleted.
 */
TEST_F(MarketTest, ExecuteOrderShouldWork4)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    uint64_t executed_quantity = 200;
    uint64_t executed_price = 300;
    market.executeOrder(symbol_id, id1, executed_quantity, executed_price);

    checkOrderAdded(id1);
    checkExecutedOrder(id1, executed_price, executed_quantity, 0);
    checkOrderDeleted(id1, executed_price, executed_quantity, 0);
    ASSERT_TRUE(market_debugger.empty());
}