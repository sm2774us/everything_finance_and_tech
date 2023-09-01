#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include "generate_orders.h"
#include "market/concurrent_market.h"
#include "event_handler/event_handler.h"

using namespace RapidTrader;

static void BM_ConcurrentMarket(benchmark::State &state)
{
    const uint64_t num_symbols = state.range(0);
    const uint64_t num_orders = state.range(1);
    const uint8_t num_threads = 3;
    std::vector<Order> orders;
    orders.reserve(num_orders);
    generateOrders(orders, num_orders, num_symbols);
    for (auto _ : state)
    {
        // Add all the symbols before adding any orders.
        state.PauseTiming();
        std::vector<std::unique_ptr<EventHandler>> event_handlers;
        event_handlers.reserve(num_threads);
        for (int i = 0; i < num_threads; ++i)
            event_handlers.push_back(std::make_unique<EventHandler>());
        ConcurrentMarket market{event_handlers, num_threads};
        for (int i = 1; i <= num_symbols; ++i)
            market.addSymbol(i, "MARKET BENCH");
        state.ResumeTiming();
        // Add all the orders.
        for (const auto &order : orders)
            market.addOrder(order);
    }
}

BENCHMARK(BM_ConcurrentMarket)
    ->Unit(benchmark::kMillisecond)
    ->Args({1, 1000000})
    ->Args({1, 2000000})
    ->Args({1, 3000000})
    ->Args({1, 4000000})
    ->ArgNames({"symbols", "orders"});
BENCHMARK(BM_ConcurrentMarket)
    ->Unit(benchmark::kMillisecond)
    ->Args({100, 1000000})
    ->Args({100, 2000000})
    ->Args({100, 3000000})
    ->Args({100, 4000000})
    ->ArgNames({"symbols", "orders"});
BENCHMARK(BM_ConcurrentMarket)
    ->Unit(benchmark::kMillisecond)
    ->Args({1000, 1000000})
    ->Args({1000, 2000000})
    ->Args({1000, 3000000})
    ->Args({1000, 4000000})
    ->ArgNames({"symbols", "orders"});
BENCHMARK(BM_ConcurrentMarket)
    ->Unit(benchmark::kMillisecond)
    ->Args({2000, 1000000})
    ->Args({2000, 2000000})
    ->Args({2000, 3000000})
    ->Args({2000, 4000000})
    ->ArgNames({"symbols", "orders"});
BENCHMARK_MAIN();