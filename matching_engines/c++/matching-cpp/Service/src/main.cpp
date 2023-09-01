#include "spdlog/async.h"
#include "spdlog/async_logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <boost/asio.hpp>
#include <chrono>
#include <ctime>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <sstream>
#include <string_view>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <sstream>
#include <orderbook.hpp>
#include <tcp_server.hpp>
#include <order_router.hpp>

using namespace std::chrono_literals;
namespace me = matching_engine;

int main(int argc, char *argv[])
{
    /* Initialise logging service */
    spdlog::init_thread_pool(32768, std::max(1u, std::thread::hardware_concurrency() - 1));
    spdlog::flush_every(1s);
    const auto console = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>("console");

    /* Initialise order dispatching service */
    const std::vector<std::string_view> markets = {
        u8"EUR_USD", u8"GBP_USD", u8"AUD_USD", u8"NZD_USD",
        u8"EUR_GBP",
        u8"USD_CHF",
        u8"USD_CAD",
        u8"EUR_AUD",
        u8"GBP_JPY", u8"USD_JPY"
    };
    auto dispatcher = std::make_shared<me::router::dispatcher>(markets, console);

    /* Initialise TCP transport layer */
    boost::asio::io_context ioc{(int)std::thread::hardware_concurrency()};
    me::tcp::server server(ioc, dispatcher, console);

    ioc.run();

    /*
     * Join threads after event loop termination
     * TODO notify market consumers to stop
     * */

    return 0;
}

/*
   const auto tick_writer = [&] {
   auto tick_logger =
   spdlog::create_async<spdlog::sinks::basic_file_sink_mt>(
   "tick_log", "data/feed.csv", true);
   tick_logger->set_pattern("%E%e,%v");
   auto pbs = ob->best_sell();
   auto pbb = ob->best_buy();
   while (true) {
   auto bb = ob->best_buy();
   auto bs = ob->best_sell();
   if ((bb && bs && pbs && pbb) && bs != pbs) {
   tick_logger->info("{},{}", bb, bs);
   }
   pbb = bb;
   pbs = bs;
   std::this_thread::sleep_for(500ms);
   }
   };
   const auto snapshot_writer = [&] {
   while (true) {
// Destroy existing snapshot file on every iteration
auto ob_logger =
spdlog::create_async<spdlog::sinks::basic_file_sink_mt>(
"orderbook_log", "data/snapshot.csv", true);
ob_logger->set_pattern("%v");
for (auto point : ob->snapshot()) {
ob_logger->info("{},{},{},{}", point.side, point.price,
point.cumulative_quantity, point.size);
}
spdlog::drop("orderbook_log");
std::this_thread::sleep_for(500ms);
}
};
boost::asio::post(pool, tick_writer);
boost::asio::post(pool, snapshot_writer);
*/