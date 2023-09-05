#include "market_test_fixture.h"

/**
 * Tests deleting an order such that it does not result in matching.
 */
TEST_F(MarketTest, DeleteOrderShouldWork1)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitAskOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    market.deleteOrder(symbol_id, id1);

    checkOrderAdded(id1);
    checkOrderDeleted(id1, 0, 0, quantity1);
    ASSERT_TRUE(market_debugger.empty());
}
