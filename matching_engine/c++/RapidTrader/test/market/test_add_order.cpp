#include "market_test_fixture.h"

/**
 * Tests adding GTC limit order to empty orderbook.
 */
TEST_F(MarketTest, AddGtcLimitOrder1)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    checkOrderAdded(id1);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding GTC limit orders that are matchable to an orderbook.
 */
TEST_F(MarketTest, AddGtcLimitOrder2)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 500;
    uint64_t price2 = 200;
    uint64_t id2 = 2;
    Order order2 = Order::limitAskOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkExecutedOrder(id2, price1, quantity1, quantity2 - quantity1);
    checkOrderDeleted(id1, price1, quantity1, 0);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding limit IOC order that is not able to be completely filled to an orderbook.
 */
TEST_F(MarketTest, AddIocLimitOrder1)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitAskOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 100;
    uint64_t price2 = 400;
    uint64_t id2 = 2;
    Order order2 = Order::limitAskOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    OrderTimeInForce tof3 = OrderTimeInForce::IOC;
    uint64_t quantity3 = 300;
    uint64_t price3 = 450;
    uint64_t id3 = 3;
    Order order3 = Order::limitBidOrder(id3, symbol_id, price3, quantity3, tof3);
    market.addOrder(order3);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkOrderAdded(id3);
    checkExecutedOrder(id3, price1, quantity1, quantity3 - quantity1);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkExecutedOrder(id3, price2, quantity2, 0);
    checkExecutedOrder(id2, price2, quantity3 - quantity1, 0);
    checkOrderDeleted(id1, price1, quantity1, 0);
    checkOrderDeleted(id2, price2, quantity2, 0);
    checkOrderDeleted(id3, price2, quantity2, 0);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding limit IOC order that is not able to be completely filled to an orderbook.
 */
TEST_F(MarketTest, AddIocLimitOrder2)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    OrderTimeInForce tof2 = OrderTimeInForce::IOC;
    uint64_t quantity2 = 300;
    uint64_t price2 = 300;
    uint64_t id2 = 2;
    Order order2 = Order::limitAskOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkExecutedOrder(id2, price1, quantity1, quantity2 - quantity1);
    checkOrderDeleted(id1, price1, quantity1, 0);
    checkOrderDeleted(id2, price1, quantity1, quantity2 - quantity1);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding limit FOK order that is able to be completely filled to an orderbook.
 */
TEST_F(MarketTest, AddFokLimitOrder1)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitAskOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 100;
    uint64_t price2 = 400;
    uint64_t id2 = 2;
    Order order2 = Order::limitAskOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    OrderTimeInForce tof3 = OrderTimeInForce::FOK;
    uint64_t quantity3 = 250;
    uint64_t price3 = 450;
    uint64_t id3 = 3;
    Order order3 = Order::limitBidOrder(id3, symbol_id, price3, quantity3, tof3);
    market.addOrder(order3);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkOrderAdded(id3);
    checkExecutedOrder(id3, price1, quantity1, quantity3 - quantity1);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkExecutedOrder(id3, price2, quantity3 - quantity1, 0);
    checkExecutedOrder(id2, price2, quantity3 - quantity1, quantity2 - (quantity3 - quantity1));
    checkOrderDeleted(id1, price1, quantity1, 0);
    checkOrderDeleted(id3, price2, quantity3 - quantity1, 0);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding limit FOK order that is not able to be completely filled to an orderbook.
 */
TEST_F(MarketTest, AddFokLimitOrder2)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 100;
    uint64_t price2 = 400;
    uint64_t id2 = 2;
    Order order2 = Order::limitBidOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    OrderTimeInForce tof3 = OrderTimeInForce::FOK;
    uint64_t quantity3 = 1000;
    uint64_t price3 = 450;
    uint64_t id3 = 3;
    Order order3 = Order::limitAskOrder(id3, symbol_id, price3, quantity3, tof3);
    market.addOrder(order3);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkOrderAdded(id3);
    checkOrderDeleted(id3, 0, 0, quantity3);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding market IOC order that is not able to be completely filled to an orderbook.
 */
TEST_F(MarketTest, AddIocMarketOrder1)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 100;
    uint64_t price2 = 250;
    uint64_t id2 = 2;
    Order order2 = Order::limitBidOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    OrderTimeInForce tof3 = OrderTimeInForce::IOC;
    uint64_t quantity3 = 500;
    uint64_t id3 = 3;
    Order order3 = Order::marketAskOrder(id3, symbol_id, quantity3, tof3);
    market.addOrder(order3);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkOrderAdded(id3);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkExecutedOrder(id3, price1, quantity1, quantity3 - quantity1);
    checkExecutedOrder(id2, price2, quantity2, 0);
    checkExecutedOrder(id3, price2, quantity2, quantity3 - quantity2 - quantity1);
    checkOrderDeleted(id1, price1, quantity1, 0);
    checkOrderDeleted(id2, price2, quantity2, 0);
    checkOrderDeleted(id3, price2, quantity2, quantity3 - quantity2 - quantity1);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding market IOC order that is able to be completely filled to an orderbook.
 */
TEST_F(MarketTest, AddIocMarketOrder2)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitAskOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    OrderTimeInForce tof2 = OrderTimeInForce::IOC;
    uint64_t quantity2 = 100;
    uint64_t id2 = 2;
    Order order2 = Order::marketBidOrder(id2, symbol_id, quantity2, tof2);
    market.addOrder(order2);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkExecutedOrder(id2, price1, quantity2, 0);
    checkExecutedOrder(id1, price1, quantity2, quantity1 - quantity2);
    checkOrderDeleted(id2, price1, quantity2, 0);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding stop IOC order that is activated when it is added to the book.
 */
TEST_F(MarketTest, AddIocStopOrder1)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 900;
    uint64_t price2 = 250;
    uint64_t id2 = 2;
    Order order2 = Order::limitAskOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    OrderTimeInForce tof3 = OrderTimeInForce::IOC;
    uint64_t quantity3 = 500;
    uint64_t price3 = 300;
    uint64_t id3 = 3;
    Order order3 = Order::stopBidOrder(id3, symbol_id, price3, quantity3, tof3);
    market.addOrder(order3);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkOrderAdded(id3);
    checkOrderUpdated(id3, 0, 0, quantity3);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkExecutedOrder(id2, price1, quantity1, quantity2 - quantity1);
    checkExecutedOrder(id3, price2, quantity3, 0);
    checkExecutedOrder(id2, price2, quantity3, quantity2 - quantity1 - quantity3);
    checkOrderDeleted(id1, price1, quantity1, 0);
    checkOrderDeleted(id3, price2, quantity3, 0);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding stop IOC order that is activated after new limit order is added to the book.
 */
TEST_F(MarketTest, AddIocStopOrder2)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 50;
    uint64_t price1 = 348;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    // Matches with last order - last traded price is now 348.
    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 50;
    uint64_t price2 = 345;
    uint64_t id2 = 2;
    Order order2 = Order::limitAskOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    // Does not activate - last traded price exceeds stop price.
    OrderTimeInForce tof3 = OrderTimeInForce::IOC;
    uint64_t quantity3 = 25;
    uint64_t price3 = 344;
    uint64_t id3 = 3;
    Order order3 = Order::stopAskOrder(id3, symbol_id, price3, quantity3, tof3);
    market.addOrder(order3);

    OrderTimeInForce tof4 = OrderTimeInForce::GTC;
    uint64_t quantity4 = 200;
    uint64_t price4 = 343;
    uint64_t id4 = 4;
    Order order4 = Order::limitBidOrder(id4, symbol_id, price4, quantity4, tof4);
    market.addOrder(order4);

    // Matches with last order - last traded price is now 343.
    // Stop order should activate - last traded price has fallen below stop price.
    OrderTimeInForce tof5 = OrderTimeInForce::GTC;
    uint64_t quantity5 = 50;
    uint64_t price5 = 342;
    uint64_t id5 = 5;
    Order order5 = Order::limitAskOrder(id5, symbol_id, price5, quantity5, tof5);
    market.addOrder(order5);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkOrderAdded(id3);
    checkOrderAdded(id4);
    checkOrderAdded(id5);
    checkOrderUpdated(id3, 0, 0, quantity3);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkExecutedOrder(id2, price1, quantity2, 0);
    checkExecutedOrder(id4, price4, quantity5, quantity4 - quantity5);
    checkExecutedOrder(id5, price4, quantity5, 0);
    checkExecutedOrder(id4, price4, quantity3, quantity4 - quantity5 - quantity3);
    checkExecutedOrder(id3, price4, quantity3, 0);
    checkOrderDeleted(id1, price1, quantity1, 0);
    checkOrderDeleted(id2, price1, quantity2, 0);
    checkOrderDeleted(id5, price4, quantity5, 0);
    checkOrderDeleted(id3, price4, quantity3, 0);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding multiple stop IOC orders that are activated after new limit order is added to the book.
 */
TEST_F(MarketTest, AddIocStopOrder3)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 25;
    uint64_t price1 = 321;
    uint64_t id1 = 1;
    Order order1 = Order::limitAskOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    // Matches with last order - last traded price is now 321.
    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 25;
    uint64_t price2 = 321;
    uint64_t id2 = 2;
    Order order2 = Order::limitBidOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    // Does not activate - stop price exceeds last traded price.
    OrderTimeInForce tof3 = OrderTimeInForce::IOC;
    uint64_t quantity3 = 100;
    uint64_t price3 = 322;
    uint64_t id3 = 3;
    Order order3 = Order::stopBidOrder(id3, symbol_id, price3, quantity3, tof3);
    market.addOrder(order3);

    // Does not activate - stop price exceeds last traded price.
    OrderTimeInForce tof4 = OrderTimeInForce::IOC;
    uint64_t quantity4 = 200;
    uint64_t price4 = 322;
    uint64_t id4 = 4;
    Order order4 = Order::stopBidOrder(id4, symbol_id, price4, quantity4, tof4);
    market.addOrder(order4);

    // Does not activate - stop price exceeds last traded price.
    OrderTimeInForce tof5 = OrderTimeInForce::IOC;
    uint64_t quantity5 = 220;
    uint64_t price5 = 323;
    uint64_t id5 = 5;
    Order order5 = Order::stopBidOrder(id5, symbol_id, price5, quantity5, tof5);
    market.addOrder(order5);

    OrderTimeInForce tof6 = OrderTimeInForce::GTC;
    uint64_t quantity6 = 50;
    uint64_t price6 = 324;
    uint64_t id6 = 6;
    Order order6 = Order::limitBidOrder(id6, symbol_id, price6, quantity6, tof6);
    market.addOrder(order6);

    // Matches with last order - last traded price is now 324.
    // Should activate all stop orders.
    OrderTimeInForce tof7 = OrderTimeInForce::GTC;
    uint64_t quantity7 = 700;
    uint64_t price7 = 321;
    uint64_t id7 = 7;
    Order order7 = Order::limitAskOrder(id7, symbol_id, price7, quantity7, tof7);
    market.addOrder(order7);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkOrderAdded(id3);
    checkOrderAdded(id4);
    checkOrderAdded(id5);
    checkOrderAdded(id6);
    checkOrderAdded(id7);
    checkOrderUpdated(id3, 0, 0, quantity3);
    checkOrderUpdated(id4, 0, 0, quantity4);
    checkOrderUpdated(id5, 0, 0, quantity5);
    checkExecutedOrder(id2, price1, quantity2, 0);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkExecutedOrder(id6, price6, quantity6, 0);
    checkExecutedOrder(id7, price6, quantity6, quantity7 - quantity6);
    checkExecutedOrder(id3, price7, quantity3, 0);
    checkExecutedOrder(id7, price7, quantity3, quantity7 - quantity6 - quantity3);
    checkExecutedOrder(id4, price7, quantity4, 0);
    checkExecutedOrder(id7, price7, quantity4, quantity7 - quantity6 - quantity3 - quantity4);
    checkExecutedOrder(id5, price7, quantity5, 0);
    checkExecutedOrder(id7, price7, quantity5, quantity7 - quantity6 - quantity3 - quantity4 - quantity5);
    checkOrderDeleted(id1, price1, quantity1, 0);
    checkOrderDeleted(id2, price1, quantity2, 0);
    checkOrderDeleted(id6, price6, quantity6, 0);
    checkOrderDeleted(id3, price7, quantity3, 0);
    checkOrderDeleted(id4, price7, quantity4, 0);
    checkOrderDeleted(id5, price7, quantity5, 0);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding stop limit GTC order that is activated when it is added to the book.
 */
TEST_F(MarketTest, AddGtcStopLimitOrder1)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitAskOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 350;
    uint64_t price2 = 400;
    uint64_t id2 = 2;
    Order order2 = Order::limitBidOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    OrderTimeInForce tof3 = OrderTimeInForce::GTC;
    uint64_t quantity3 = 500;
    uint64_t stop_price = 300;
    uint64_t price3 = 500;
    uint64_t id3 = 3;
    Order order3 = Order::stopLimitBidOrder(id3, symbol_id, price3, stop_price, quantity3, tof3);
    market.addOrder(order3);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkOrderAdded(id3);
    checkOrderUpdated(id3, 0, 0, quantity3);
    checkExecutedOrder(id2, price1, quantity1, quantity2 - quantity1);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkOrderDeleted(id1, price1, quantity1, 0);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding stop limit GTC order that is activated after a trade.
 */
TEST_F(MarketTest, AddGtcStopLimitOrder2)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitAskOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 350;
    uint64_t price2 = 400;
    uint64_t id2 = 2;
    Order order2 = Order::limitBidOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    OrderTimeInForce tof3 = OrderTimeInForce::GTC;
    uint64_t quantity3 = 500;
    uint64_t stop_price = 375;
    uint64_t price3 = 500;
    uint64_t id3 = 3;
    Order order3 = Order::stopLimitAskOrder(id3, symbol_id, price3, stop_price, quantity3, tof3);
    market.addOrder(order3);

    OrderTimeInForce tof4 = OrderTimeInForce::GTC;
    uint64_t quantity4 = 50;
    uint64_t price4 = 380;
    uint64_t id4 = 1;
    Order order4 = Order::limitAskOrder(id4, symbol_id, price4, quantity4, tof4);
    market.addOrder(order4);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkOrderAdded(id3);
    checkOrderAdded(id4);
    checkOrderUpdated(id3, 0, 0, quantity3);
    checkExecutedOrder(id2, price1, quantity1, quantity2 - quantity1);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkExecutedOrder(id2, price2, quantity4, quantity2 - quantity1 - quantity4);
    checkExecutedOrder(id4, price2, quantity4, 0);
    checkOrderDeleted(id1, price1, quantity1, 0);
    checkOrderDeleted(id4, price2, quantity4, 0);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding trailing stop IOC order to the book that is activated after a trade.
 */
TEST_F(MarketTest, AddIocTrailingStopOrder1)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 170;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    // Matches with last order - last traded price is now 170.
    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 200;
    uint64_t price2 = 170;
    uint64_t id2 = 2;
    Order order2 = Order::limitAskOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    // Last traded price is 170.
    // Trail amount is 1.
    // Inserted into the book with stop price last traded price + trail amount = 169.
    OrderTimeInForce tof3 = OrderTimeInForce::IOC;
    uint64_t quantity3 = 50;
    uint64_t trail_amount = 1;
    uint64_t id3 = 3;
    Order order3 = Order::trailingStopAskOrder(id3, symbol_id, trail_amount, quantity3, tof3);
    market.addOrder(order3);

    OrderTimeInForce tof4 = OrderTimeInForce::GTC;
    uint64_t quantity4 = 100;
    uint64_t price4 = 168;
    uint64_t id4 = 4;
    Order order4 = Order::limitAskOrder(id4, symbol_id, price4, quantity4, tof4);
    market.addOrder(order4);

    // Matches with last order - last traded price is now 168.
    // Trailing stop order should be activated.
    OrderTimeInForce tof5 = OrderTimeInForce::GTC;
    uint64_t quantity5 = 200;
    uint64_t price5 = 170;
    uint64_t id5 = 5;
    Order order5 = Order::limitBidOrder(id5, symbol_id, price5, quantity5, tof5);
    market.addOrder(order5);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkOrderAdded(id3);
    checkOrderAdded(id4);
    checkOrderAdded(id5);
    checkOrderUpdated(id3, 0, 0, quantity3);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkExecutedOrder(id2, price1, quantity2, 0);
    checkExecutedOrder(id5, price4, quantity4, quantity5 - quantity4);
    checkExecutedOrder(id4, price4, quantity4, 0);
    checkExecutedOrder(id5, price5, quantity3, quantity5 - quantity4 - quantity3);
    checkExecutedOrder(id3, price5, quantity3, 0);
    checkOrderDeleted(id1, price1, quantity1, 0);
    checkOrderDeleted(id2, price1, quantity2, 0);
    checkOrderDeleted(id4, price4, quantity4, 0);
    checkOrderDeleted(id3, price5, quantity3, 0);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding trailing stop IOC order to the book that is activated after another
 * stop order was activated and traded.
 */
TEST_F(MarketTest, AddIocTrailingStopOrder2)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 170;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    // Matches with last order - last traded price is now 170.
    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 200;
    uint64_t price2 = 170;
    uint64_t id2 = 2;
    Order order2 = Order::limitAskOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    // Last traded price is 170.
    // Trail amount is 3.
    // Inserted into book with stop price = last traded price + trail amount = 173.
    OrderTimeInForce tof3 = OrderTimeInForce::IOC;
    uint64_t quantity3 = 50;
    uint64_t price3 = 175;
    uint64_t trail_amount1 = 3;
    uint64_t id3 = 3;
    Order order3 = Order::trailingStopBidOrder(id3, symbol_id, trail_amount1, quantity3, tof3);
    market.addOrder(order3);

    // Last traded price is 170.
    // Trail amount is 2.
    // Inserted into book with stop price = last traded price + trail amount = 172.
    OrderTimeInForce tof4 = OrderTimeInForce::IOC;
    uint64_t quantity4 = 100;
    uint64_t price4 = 175;
    uint64_t trail_amount2 = 2;
    uint64_t id4 = 4;
    Order order4 = Order::trailingStopBidOrder(id4, symbol_id, trail_amount2, quantity4, tof4);
    market.addOrder(order4);

    OrderTimeInForce tof5 = OrderTimeInForce::GTC;
    uint64_t quantity5 = 100;
    uint64_t price5 = 168;
    uint64_t id5 = 5;
    Order order5 = Order::limitAskOrder(id5, symbol_id, price5, quantity5, tof5);
    market.addOrder(order5);

    // Matches with last order - traded price is now 168.
    // Stop price of trailing stops should be adjusted.
    // First trailing stop order should have stop price = last traded price + trail amount = 171.
    // Second trailing stop order should have stop price = last traded price + trail amount = 170.
    OrderTimeInForce tof6 = OrderTimeInForce::GTC;
    uint64_t quantity6 = 100;
    uint64_t price6 = 170;
    uint64_t id6 = 6;
    Order order6 = Order::limitBidOrder(id6, symbol_id, price6, quantity6, tof6);
    market.addOrder(order6);

    OrderTimeInForce tof7 = OrderTimeInForce::GTC;
    uint64_t quantity7 = 700;
    uint64_t price7 = 172;
    uint64_t id7 = 7;
    Order order7 = Order::limitAskOrder(id7, symbol_id, price7, quantity7, tof7);
    market.addOrder(order7);

    OrderTimeInForce tof8 = OrderTimeInForce::GTC;
    uint64_t quantity8 = 200;
    uint64_t price8 = 170;
    uint64_t id8 = 8;
    Order order8 = Order::limitAskOrder(id8, symbol_id, price8, quantity8, tof8);
    market.addOrder(order8);

    // Matches with last order - last traded price is now 170.
    // This should activate the stop order with stop price 170.
    // That stop order should match with ask order with price 172.
    // This should activate the stop order with price 171.
    OrderTimeInForce tof9 = OrderTimeInForce::GTC;
    uint64_t quantity9 = 200;
    uint64_t price9 = 170;
    uint64_t id9 = 9;
    Order order9 = Order::limitBidOrder(id9, symbol_id, price9, quantity9, tof9);
    market.addOrder(order9);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkOrderAdded(id3);
    checkOrderAdded(id4);
    checkOrderAdded(id5);
    checkOrderAdded(id6);
    checkOrderAdded(id7);
    checkOrderAdded(id8);
    checkOrderAdded(id9);
    checkOrderUpdated(id4, 0, 0, quantity4);
    checkOrderUpdated(id3, 0, 0, quantity3);
    checkOrderUpdated(id4, 0, 0, quantity4);
    checkOrderUpdated(id3, 0, 0, quantity3);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkExecutedOrder(id2, price1, quantity2, 0);
    checkExecutedOrder(id6, price5, quantity6, 0);
    checkExecutedOrder(id5, price5, quantity5, 0);
    checkExecutedOrder(id9, price8, quantity9, 0);
    checkExecutedOrder(id8, price8, quantity8, 0);
    checkExecutedOrder(id4, price7, quantity4, 0);
    checkExecutedOrder(id7, price7, quantity4, quantity7 - quantity4);
    checkExecutedOrder(id3, price7, quantity3, 0);
    checkExecutedOrder(id7, price7, quantity3, quantity7 - quantity4 - quantity3);
    checkOrderDeleted(id1, price1, quantity1, 0);
    checkOrderDeleted(id2, price1, quantity2, 0);
    checkOrderDeleted(id5, price5, quantity5, 0);
    checkOrderDeleted(id6, price5, quantity6, 0);
    checkOrderDeleted(id8, price8, quantity8, 0);
    checkOrderDeleted(id9, price8, quantity9, 0);
    checkOrderDeleted(id4, price7, quantity4, 0);
    checkOrderDeleted(id3, price7, quantity3, 0);
    ASSERT_TRUE(market_debugger.empty());
}

/**
 * Tests adding trailing stop limit GTC order that is activated after a trade.
 */
TEST_F(MarketTest, AddGtcTrailingStopLimitOrder1)
{
    OrderTimeInForce tof1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 170;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol_id, price1, quantity1, tof1);
    market.addOrder(order1);

    // Matches with last order - last traded price is now 170.
    OrderTimeInForce tof2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 200;
    uint64_t price2 = 170;
    uint64_t id2 = 2;
    Order order2 = Order::limitAskOrder(id2, symbol_id, price2, quantity2, tof2);
    market.addOrder(order2);

    // Last traded price is 170.
    // Trail amount is 2.
    // Inserted into book with stop price = last traded price + trail amount = 172.
    OrderTimeInForce tof3 = OrderTimeInForce::GTC;
    uint64_t quantity3 = 50;
    uint64_t price3 = 140;
    uint64_t trail_amount1 = 2;
    uint64_t id3 = 3;
    Order order3 = Order::trailingStopLimitBidOrder(id3, symbol_id, price3, trail_amount1, quantity3, tof3);
    market.addOrder(order3);

    // Last traded price is 170.
    // Trail amount is 2.
    // Inserted into book with stop price = last traded price + trail amount = 172.
    OrderTimeInForce tof4 = OrderTimeInForce::GTC;
    uint64_t quantity4 = 100;
    uint64_t price4 = 145;
    uint64_t trail_amount2 = 2;
    uint64_t id4 = 4;
    Order order4 = Order::trailingStopLimitBidOrder(id4, symbol_id, price4, trail_amount2, quantity4, tof4);
    market.addOrder(order4);

    OrderTimeInForce tof5 = OrderTimeInForce::GTC;
    uint64_t quantity5 = 100;
    uint64_t price5 = 168;
    uint64_t id5 = 5;
    Order order5 = Order::limitAskOrder(id5, symbol_id, price5, quantity5, tof5);
    market.addOrder(order5);

    // Matches with last order - traded price is now 168.
    // Stop price of trailing stops should be adjusted.
    // First trailing stop order should have stop price = last traded price + trail amount = 170.
    // Second trailing stop order should have stop price = last traded price + trail amount = 170.
    OrderTimeInForce tof6 = OrderTimeInForce::GTC;
    uint64_t quantity6 = 100;
    uint64_t price6 = 170;
    uint64_t id6 = 6;
    Order order6 = Order::limitBidOrder(id6, symbol_id, price6, quantity6, tof6);
    market.addOrder(order6);

    OrderTimeInForce tof7 = OrderTimeInForce::GTC;
    uint64_t quantity7 = 700;
    uint64_t price7 = 172;
    uint64_t id7 = 7;
    Order order7 = Order::limitAskOrder(id7, symbol_id, price7, quantity7, tof7);
    market.addOrder(order7);

    OrderTimeInForce tof8 = OrderTimeInForce::GTC;
    uint64_t quantity8 = 200;
    uint64_t price8 = 170;
    uint64_t id8 = 8;
    Order order8 = Order::limitAskOrder(id8, symbol_id, price8, quantity8, tof8);
    market.addOrder(order8);

    // Matches with last order - last traded price is now 170.
    // This should activate both stop orders.
    OrderTimeInForce tof9 = OrderTimeInForce::GTC;
    uint64_t quantity9 = 200;
    uint64_t price9 = 170;
    uint64_t id9 = 9;
    Order order9 = Order::limitBidOrder(id9, symbol_id, price9, quantity9, tof9);
    market.addOrder(order9);

    checkOrderAdded(id1);
    checkOrderAdded(id2);
    checkOrderAdded(id3);
    checkOrderAdded(id4);
    checkOrderAdded(id5);
    checkOrderAdded(id6);
    checkOrderAdded(id7);
    checkOrderAdded(id8);
    checkOrderAdded(id9);
    checkOrderUpdated(id3, 0, 0, quantity3);
    checkOrderUpdated(id4, 0, 0, quantity4);
    checkOrderUpdated(id3, 0, 0, quantity3);
    checkOrderUpdated(id4, 0, 0, quantity4);
    checkExecutedOrder(id1, price1, quantity1, 0);
    checkExecutedOrder(id2, price1, quantity2, 0);
    checkExecutedOrder(id6, price5, quantity6, 0);
    checkExecutedOrder(id5, price5, quantity5, 0);
    checkExecutedOrder(id9, price8, quantity9, 0);
    checkExecutedOrder(id8, price8, quantity8, 0);
    checkOrderDeleted(id1, price1, quantity1, 0);
    checkOrderDeleted(id2, price1, quantity2, 0);
    checkOrderDeleted(id5, price5, quantity5, 0);
    checkOrderDeleted(id6, price5, quantity6, 0);
    checkOrderDeleted(id8, price8, quantity8, 0);
    checkOrderDeleted(id9, price8, quantity9, 0);
    ASSERT_TRUE(market_debugger.empty());
}