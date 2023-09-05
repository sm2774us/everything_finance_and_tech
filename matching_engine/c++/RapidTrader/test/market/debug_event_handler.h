#ifndef RAPID_TRADER_DEBUG_EVENT_HANDLER_H
#define RAPID_TRADER_DEBUG_EVENT_HANDLER_H
#include "event_handler/event_handler.h"
#include "event.h"

using namespace RapidTrader;

struct MarketEventDebugger
{
    std::queue<OrderAdded> add_order_events;
    std::queue<OrderDeleted> delete_order_events;
    std::queue<ExecutedOrder> execute_order_events;
    std::queue<OrderUpdated> update_order_events;
    std::queue<SymbolAdded> add_symbol_events;
    std::queue<SymbolDeleted> delete_symbol_events;

    [[nodiscard]] bool empty() const
    {
        return add_order_events.empty() && delete_order_events.empty() && execute_order_events.empty() && update_order_events.empty() &&
               add_symbol_events.empty() && delete_symbol_events.empty();
    }
};

class DebugEventHandler : public EventHandler
{

public:
    explicit DebugEventHandler(MarketEventDebugger &market_debugger_)
        : market_debugger(market_debugger_)
    {}

protected:
    void handleOrderAdded(const OrderAdded &notification) override
    {
        market_debugger.add_order_events.push(notification);
    }
    void handleOrderDeleted(const OrderDeleted &notification) override
    {
        market_debugger.delete_order_events.push(notification);
    }
    void handleOrderUpdated(const OrderUpdated &notification) override
    {
        market_debugger.update_order_events.push(notification);
    }
    void handleOrderExecuted(const ExecutedOrder &notification) override
    {
        market_debugger.execute_order_events.push(notification);
    }
    void handleSymbolAdded(const SymbolAdded &notification) override
    {
        market_debugger.add_symbol_events.push(notification);
    }

    void handleSymbolDeleted(const SymbolDeleted &notification) override
    {
        market_debugger.delete_symbol_events.push(notification);
    }

private:
    MarketEventDebugger &market_debugger;
};
#endif // RAPID_TRADER_DEBUG_EVENT_HANDLER_H
