#include <iostream>
#include "market/concurrent_market.h"
#include "simple_event_handler.h"

using namespace RapidTrader;

int main()
{
    // Create a new concurrent market that uses 2 threads.
    // Note that there must be one event handler per thread.
    const uint32_t num_threads = 2;
    std::vector<std::unique_ptr<EventHandler>> event_handlers;
    for (auto i = 0; i < num_threads; ++i)
        event_handlers.push_back(std::unique_ptr<EventHandler>(new SimpleEventHandler));
    ConcurrentMarket market{event_handlers, num_threads};

    // Add a new symbol to the market. A symbol must be added to the market before any orders with that symbol are submitted.
    std::string symbol1_name = "GOOG";
    uint32_t symbol1_id = 1;
    market.addSymbol(symbol1_id, symbol1_name);

    // Add bid limit order to the market.
    OrderTimeInForce time_in_force1 = OrderTimeInForce::GTC;
    uint64_t quantity1 = 200;
    uint64_t price1 = 350;
    uint64_t id1 = 1;
    Order order1 = Order::limitBidOrder(id1, symbol1_id, price1, quantity1, time_in_force1);
    market.addOrder(order1);

    // Add ask limit order to the market.
    OrderTimeInForce time_in_force2 = OrderTimeInForce::GTC;
    uint64_t quantity2 = 200;
    uint64_t price2 = 350;
    uint64_t id2 = 2;
    Order order2 = Order::limitAskOrder(id2, symbol1_id, price2, quantity2, time_in_force2);
    market.addOrder(order2);

    return 0;
}
