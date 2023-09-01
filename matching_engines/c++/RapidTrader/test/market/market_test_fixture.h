#ifndef RAPID_TRADER_MARKET_TEST_FIXTURE_H
#define RAPID_TRADER_MARKET_TEST_FIXTURE_H
#include <gtest/gtest.h>
#include "debug_event_handler.h"
#include "market/market.h"

using namespace RapidTrader;

class MarketTest : public ::testing::Test
{

protected:
    void SetUp() override
    {
        market.addSymbol(symbol_id, symbol_name);
        assert(market.hasSymbol(symbol_id) && "Symbol was not added to market!");
        assert(!market_debugger.add_symbol_events.empty() && "There should have been a symbol added event!");
        market_debugger.add_symbol_events.pop();
        assert(market_debugger.empty() && "There should not be any notifications!");
    }

    static void checkOrder(const Order &order, uint64_t expected_order_id, uint64_t expected_last_execution_price,
        uint64_t expected_last_execution_quantity, uint64_t expected_open_quantity)
    {
        ASSERT_EQ(order.getOrderID(), expected_order_id);
        ASSERT_EQ(order.getLastExecutedPrice(), expected_last_execution_price);
        ASSERT_EQ(order.getLastExecutedQuantity(), expected_last_execution_quantity);
        ASSERT_EQ(order.getOpenQuantity(), expected_open_quantity);
    }

    void checkOrderAdded(uint64_t expected_order_id)
    {
        ASSERT_FALSE(market_debugger.add_order_events.empty());
        Order &order_added = market_debugger.add_order_events.front().order;
        ASSERT_EQ(order_added.getOrderID(), expected_order_id);
        market_debugger.add_order_events.pop();
    }

    void checkExecutedOrder(uint64_t expected_order_id, uint64_t expected_last_execution_price, uint64_t expected_last_execution_quantity,
        uint64_t expected_open_quantity)
    {
        ASSERT_FALSE(market_debugger.execute_order_events.empty());
        Order &order_executed = market_debugger.execute_order_events.front().order;
        checkOrder(
            order_executed, expected_order_id, expected_last_execution_price, expected_last_execution_quantity, expected_open_quantity);
        market_debugger.execute_order_events.pop();
    }

    void checkOrderDeleted(uint64_t expected_order_id, uint64_t expected_last_execution_price, uint64_t expected_last_execution_quantity,
        uint64_t expected_open_quantity)
    {
        ASSERT_FALSE(market_debugger.delete_order_events.empty());
        Order &order_deleted = market_debugger.delete_order_events.front().order;
        checkOrder(
            order_deleted, expected_order_id, expected_last_execution_price, expected_last_execution_quantity, expected_open_quantity);
        market_debugger.delete_order_events.pop();
    }

    void checkOrderUpdated(uint64_t expected_order_id, uint64_t expected_last_execution_price, uint64_t expected_last_execution_quantity,
        uint64_t expected_open_quantity)
    {
        ASSERT_FALSE(market_debugger.update_order_events.empty());
        Order &order_updated = market_debugger.update_order_events.front().order;
        checkOrder(
            order_updated, expected_order_id, expected_last_execution_price, expected_last_execution_quantity, expected_open_quantity);
        market_debugger.update_order_events.pop();
    }

    void checkSymbolAdded(uint64_t expected_symbol_id, const std::string &expected_symbol_name)
    {
        ASSERT_FALSE(market_debugger.add_symbol_events.empty());
        SymbolAdded &symbol_added_event = market_debugger.add_symbol_events.front();
        ASSERT_EQ(symbol_added_event.symbol_id, expected_symbol_id);
        ASSERT_EQ(symbol_added_event.name, expected_symbol_name);
        ASSERT_TRUE(market.hasSymbol(symbol_id));
        market_debugger.add_symbol_events.pop();
    }

    MarketEventDebugger market_debugger;
    RapidTrader::Market market{std::unique_ptr<RapidTrader::EventHandler>(new DebugEventHandler(market_debugger))};
    uint32_t symbol_id = 1;
    std::string symbol_name = "GOOG";
};
#endif // RAPID_TRADER_MARKET_TEST_FIXTURE_H
