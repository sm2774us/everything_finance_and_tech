#include <gtest/gtest.h>
#include "debug_event_handler.h"
#include "market/market.h"

/**
 * Tests deleting a symbol that exist in the market.
 */
TEST(DeleteSymbolTest, DeleteSymbolTest1)
{
    MarketEventDebugger event_debugger;
    Market market{std::make_unique<DebugEventHandler>(event_debugger)};

    std::string symbol_name = "GOOG";
    uint32_t symbol_id = 1;
    market.addSymbol(symbol_id, symbol_name);

    ASSERT_TRUE(market.hasSymbol(symbol_id));
    ASSERT_FALSE(event_debugger.add_symbol_events.empty());
    ASSERT_TRUE(event_debugger.add_symbol_events.front().symbol_id == symbol_id);
    ASSERT_TRUE(event_debugger.add_symbol_events.front().name == symbol_name);
    event_debugger.add_symbol_events.pop();
    ASSERT_TRUE(event_debugger.empty());

    market.deleteSymbol(symbol_id);
    ASSERT_FALSE(market.hasSymbol(symbol_id));
    ASSERT_FALSE(event_debugger.delete_symbol_events.empty());
    ASSERT_TRUE(event_debugger.delete_symbol_events.front().symbol_id == symbol_id);
    ASSERT_TRUE(event_debugger.delete_symbol_events.front().name == symbol_name);
    event_debugger.delete_symbol_events.pop();
    ASSERT_TRUE(event_debugger.empty());
}