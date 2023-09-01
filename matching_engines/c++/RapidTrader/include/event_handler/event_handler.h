#ifndef RAPID_TRADER_EVENT_HANDLER_H
#define RAPID_TRADER_EVENT_HANDLER_H
#include "market/market.h"
#include "market/concurrent_market.h"
#include "event_handler/event.h"

namespace RapidTrader {
class EventHandler
{
public:
    EventHandler(const EventHandler &) = delete;
    EventHandler(EventHandler &&) = delete;
    EventHandler &operator=(const EventHandler &) = delete;
    EventHandler &operator=(EventHandler &&) = delete;
    EventHandler() = default;
    virtual ~EventHandler() = default;

    friend class OrderBookHandler;
    friend class MapOrderBook;

protected:
    // LCOV_EXCL_START
    /**
     * Handles an event where an order was added.
     *
     * @param event an event where an order was added.
     */
    virtual void handleOrderAdded(const OrderAdded &event) {}

    /**
     * Handles an event where an order was deleted.
     *
     * @param event an event where an order was deleted.
     */
    virtual void handleOrderDeleted(const OrderDeleted &event) {}

    /**
     * Handles a event where an order was updated.
     *
     * @param event an event where an order was updated.
     */
    virtual void handleOrderUpdated(const OrderUpdated &event) {}

    /**
     * Handles an event where an order was executed.
     *
     * @param event an event where an order was executed.
     */
    virtual void handleOrderExecuted(const ExecutedOrder &event) {}

    /**
     * Handles an event where a symbol was added.
     *
     * @param event an event where a symbol was added.
     */
    virtual void handleSymbolAdded(const SymbolAdded &event) {}

    /**
     * Handles a event where a symbol was deleted.
     *
     * @param event an event where a symbol was deleted.
     */
    virtual void handleSymbolDeleted(const SymbolDeleted &event) {}
    // LCOV_EXCL_STOP
};
} // namespace RapidTrader
#endif // RAPID_TRADER_EVENT_HANDLER_H
