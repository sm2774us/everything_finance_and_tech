#include <order_matcher/order.h>
#include <order_matcher/security_manager.h>
#include <order_matcher/order_book.h>
#include <string>
#include <queue>

TEST(OrderMatcher, OrderBook)
{
    auto securityId = order_matcher::SecurityManager::getInstance()->addSecurity("TEST");

    int clientOrderID = 1;
    int sessionId = 0;
    std::string client = "test_client";
    std::string server = "test_server";
    order_matcher::OrderSide buySide = order_matcher::OrderSide::BUY;
    order_matcher::OrderSide sellSide = order_matcher::OrderSide::SELL;
    order_matcher::OrderType orderType = order_matcher::OrderType::LIMIT;

    double supplierPrice = 100;
    long supplierQuantity = 2;

    double buyer1Price = 200;
    double buyer1Quantity =1;

    double buyer2Price = 100;
    double buyer2Quantity = 1;

    order_matcher::Order orderSell(clientOrderID, "", securityId, sessionId, sellSide, orderType, supplierPrice, supplierQuantity);
    order_matcher::Order orderBuy1(clientOrderID, "", securityId, sessionId, buySide, orderType, buyer1Price, buyer1Quantity);
    order_matcher::Order orderBuy2(clientOrderID, "", securityId, sessionId, buySide, orderType, buyer2Price, buyer2Quantity);

    order_matcher::OrderBook book;

    book.insert(orderSell);
    book.insert(orderBuy1);

    {
        std::queue<order_matcher::Order> processedOrders;
        book.processMatching(processedOrders);
        EXPECT_EQ(2, processedOrders.size());
    }

    book.insert(orderBuy2);

    {
        std::queue<order_matcher::Order> processedOrders;
        book.processMatching(processedOrders);
        EXPECT_EQ(2, processedOrders.size());
    }

    EXPECT_EQ(true, book.isEmpty());
}