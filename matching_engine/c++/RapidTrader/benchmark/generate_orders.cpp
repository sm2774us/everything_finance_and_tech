#include "generate_orders.h"

using namespace RapidTrader;

void generateOrders(std::vector<Order> &orders, uint32_t num_orders, uint32_t num_symbols)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> uniform_dist{0, 10};
    std::uniform_int_distribution<uint64_t> symbol_dist{1, num_symbols};
    for (auto i = 1; i <= num_orders; ++i)
    {
        OrderSide side = i % 2 == 0 ? OrderSide::Ask : OrderSide::Bid;
        OrderTimeInForce tof = OrderTimeInForce::GTC;
        uint64_t price_delta = side == OrderSide::Ask ? 1005 : 1000;
        uint64_t price = uniform_dist(gen) + price_delta;
        uint64_t quantity = (uniform_dist(gen) + 1) * 100;
        uint64_t order_id = i;
        uint64_t symbol_id = symbol_dist(gen);
        orders.push_back(side == OrderSide::Ask ? Order::limitAskOrder(order_id, symbol_id, price, quantity, tof)
                                                : Order::limitBidOrder(order_id, symbol_id, price, quantity, tof));
    }
}
