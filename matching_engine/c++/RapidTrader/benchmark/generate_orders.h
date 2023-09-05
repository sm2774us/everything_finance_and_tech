#ifndef RAPID_TRADER_GENERATE_ORDERS_H
#define RAPID_TRADER_GENERATE_ORDERS_H
#include <vector>
#include <random>
#include "order.h"
using namespace RapidTrader;
void generateOrders(std::vector<Order> &orders, uint32_t num_orders, uint32_t num_symbols);
#endif // RAPID_TRADER_GENERATE_ORDERS_H
