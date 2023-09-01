#include <benchmark/benchmark.h>
#include <vector>
#include <orderbook.hpp>
#include <order_router.hpp>
#include <tcp_server.hpp>
#include "markov.h"

using namespace matching_engine;

static auto SimulateMarket(int count)
{
    double S0 = 80;
    double mu = double(rand() % 5 + 1) / 100 * (rand() % 2 ? 1 : -1); // add rand drift
    double sigma = 0.08 + double(rand() % 2 + 1) / 1000; // flat vol + rand
    double T = 1;
    return geoBrownian(S0, mu, sigma, T, count);
}

static void OrderCreation(benchmark::State& state)
{
    auto market = "USD_JPY";
    auto side = rand() % 2 ? SIDE::BUY : SIDE::SELL;
    Price price = rand() % 100 + 1;
    Quantity quantity = double(rand() % 100 + 1) / (rand() % 20 + 1);
    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        auto order = std::make_unique<Order>(market, side, price, quantity);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_seconds =
            std::chrono::duration_cast<std::chrono::duration<double>>(
                end - start);
        state.SetIterationTime(elapsed_seconds.count());
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(OrderCreation)->DenseRange(1, 1000, 250)->UseManualTime()->Complexity(benchmark::oN);

static void OrderMatching(benchmark::State& state)
{
    // Perform setup here
    std::string market = "USD_JPY";
    OrderBook ob(market);
    auto prices = SimulateMarket(state.range(0));
    for(auto _ : state) {
        for (auto price : prices) {
            auto side = rand() % 2 ? SIDE::BUY : SIDE::SELL;
            auto quantity = double(rand() % 10 + 1) / (rand() % 20 + 1);
            auto order = std::make_unique<Order>(market, side, price, quantity);
            auto start = std::chrono::high_resolution_clock::now();
            ob.match(std::move(order));
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed_seconds =
                std::chrono::duration_cast<std::chrono::duration<double>>(
                    end - start);
            state.SetIterationTime(elapsed_seconds.count());
        }
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(OrderMatching)->DenseRange(1, 1000, 250)->UseManualTime()->Complexity(benchmark::oLogN);

// TODO Starts and hangs, issue with thread pool. Fix it.
static void OrderDispatching(benchmark::State& state)
{
    // Perform setup here
    const std::vector<std::string_view> markets = {u8"USD_JPY"};
    auto dispatcher = std::make_shared<router::dispatcher>(markets);
    auto prices = SimulateMarket(state.range(0));
    for(auto _ : state) {
        for (auto price : prices) {
            auto side = rand() % 2 ? SIDE::BUY : SIDE::SELL;
            auto quantity = double(rand() % 10 + 1) / (rand() % 20 + 1);
            auto order = std::make_unique<Order>(markets[0], side, price, quantity);
            dispatcher->send(std::move(order));
        }
    }
    dispatcher->shutdown();
}

//BENCHMARK(OrderDispatching)->DenseRange(0, 1000, 250)->MeasureProcessCPUTime();

/* Run the benchmark */
BENCHMARK_MAIN();
