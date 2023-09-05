#pragma once

#include <orderbook.hpp>
#include "influxdb.hpp"
#include "spdlog/spdlog.h"
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <map>
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <tbb/concurrent_queue.h>
#include <moodycamel/blockingconcurrentqueue.h>


namespace matching_engine
{
namespace router
{
class consumer
{
    using ms = std::chrono::milliseconds;
    using ns = std::chrono::nanoseconds;
public:
    consumer(std::shared_ptr<spdlog::logger> console):
        should_exit_{false}, console_(console) {}
    consumer(const consumer&) = delete;
    consumer() = delete;
    void shutdown()
    {
        should_exit_ = true;
    }
    void push(OrderPtr order)
    {
        queue_.enqueue(std::move(order));
    }
    void register_market(std::string_view market)
    {
        markets.emplace(market, market);
    }
    void listen()
    {
        if (console_ != nullptr) {
            for (const auto& [name, _] : markets) {
                console_->info("Consumer of {} started @{}", name, (pid_t) syscall (SYS_gettid));
            }
        }
        OrderPtr order;
        auto last_log = Time::now();
        while(should_consume_()) {
            queue_.wait_dequeue(order);
            auto &ob = markets.at(order->market_name());
            const auto start = Time::now();
            ob.match(std::move(order));
            auto elapsed = Time::now() - start;
            /* Post consumer stats */
            if (std::chrono::duration_cast<ms>(start.time_since_epoch()).count()
                - std::chrono::duration_cast<ms>(last_log.time_since_epoch()).count() >= 250) {
                std::async(std::launch::async,[&,market=ob.market_name(),elapsed] {
                    /* NOTE Does it actually make sense to use async here?*/
                    influxdb_cpp::builder()
                    .meas("order_matcher")
                    .tag("market", market.data())
                    .field("execution_duration", std::chrono::duration_cast<ns>(elapsed).count())
                    .field("consumer_queue_length", static_cast<long>(queue_.size_approx()))
                    .timestamp(std::chrono::duration_cast<ns>(Time::now().time_since_epoch()).count())
                    .send_udp("172.17.0.1", 8089);
                });
                last_log = start;
            }

        }
    }
private:
    bool should_consume_() const
    {
        return !should_exit_ or queue_.size_approx() > 0;
    }
    std::unordered_map<std::string_view, OrderBook> markets;
    moodycamel::BlockingConcurrentQueue<OrderPtr> queue_;
    std::atomic_bool should_exit_;
    std::shared_ptr<spdlog::logger> console_;
};

class dispatcher
{
public:
    dispatcher() = default;
    dispatcher(const dispatcher&) = delete;
    dispatcher(std::vector<std::string_view> markets,
               std::shared_ptr<spdlog::logger> console = nullptr,
               const uint64_t available_cores = std::max(1u, std::thread::hardware_concurrency() - 1)):
        pool_{available_cores},
        console_{console}
    {
        const auto markets_per_core = uint64_t(markets.size() / available_cores);
        auto reminder = markets.size() % available_cores;
        std::vector<std::shared_ptr<consumer>> consumer_pool;
        for (unsigned int core = 0; core < available_cores; core++) {
            consumer_pool.emplace_back(std::make_shared<consumer>(console_));
            auto& market_consumer = consumer_pool.back();
            if (reminder > 0) {
                auto market = markets.back();
                market_registry_[market] = market_consumer;
                market_consumer->register_market(market);
                markets.pop_back();
                --reminder;
            }
            /* Costruct consumer for N markets distributed evenly across logical cores */
            for (auto index = markets_per_core; index > 0; --index) {
                auto market = markets.back();
                market_registry_[market] = market_consumer;
                market_consumer->register_market(market);
                markets.pop_back();
            }
        }
        for (auto& c : consumer_pool) {
            boost::asio::post(pool_, std::bind(&consumer::listen, c));
        }
    }
    void send(OrderPtr order)
    {
        auto& market_consumer = market_registry_.at(order->market_name());
        market_consumer->push(std::move(order));
    }
    std::string_view registered_market_name(const std::string_view market) const
    {
        return market_registry_.find(market)->first;
    }
    void shutdown()
    {
        for (auto const& [_, c] : market_registry_) {
            c->shutdown();
        }
        pool_.join();
    }
private:
    std::unordered_map<std::string_view, std::shared_ptr<consumer>> market_registry_;
    boost::asio::thread_pool pool_;
    std::shared_ptr<spdlog::logger> console_;
};
} // namespace router
} // namespace matching_engine
