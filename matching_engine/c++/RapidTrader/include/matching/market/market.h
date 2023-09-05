#ifndef RAPID_TRADER_MARKET_H
#define RAPID_TRADER_MARKET_H
#include <iostream>
#include <fstream>
#include <memory>
#include "utils/log.h"
#include "utils/robin_hood.h"
#include "order.h"
#include "orderbook.h"
#include "symbol.h"
#include "event_handler.h"

namespace RapidTrader {
class EventHandler;

// A struct for the shared logic between market and concurrent market.
// Necessary to prevent race condition in concurrent market.
struct OrderBookHandler
{
    explicit OrderBookHandler(std::unique_ptr<EventHandler> event_handler);

    void addOrderBook(uint32_t symbol_id, std::string symbol_name);

    void deleteOrderBook(uint32_t symbol_id, std::string symbol_name);

    void addOrder(const Order &order);

    void deleteOrder(uint32_t symbol_id, uint64_t order_id);

    void cancelOrder(uint32_t symbol_id, uint64_t order_id, uint64_t cancelled_quantity);

    void replaceOrder(uint32_t symbol_id, uint64_t order_id, uint64_t new_order_id, uint64_t new_price);

    void executeOrder(uint32_t symbol_id, uint64_t order_id, uint64_t quantity, uint64_t price);

    void executeOrder(uint32_t symbol_id, uint64_t order_id, uint64_t quantity);

    std::string toString();

private:
    // Maps symbol IDs to order books.
    robin_hood::unordered_map<uint32_t, std::unique_ptr<OrderBook>> id_to_book;
    // Handles orderbook events.
    std::unique_ptr<EventHandler> event_handler;
};

class Market
{
public:
    Market(Market &&other) = delete;
    Market &operator=(Market &&other) = delete;

    /**
     * A constructor for the Market.
     *
     * @param outgoing_messages_ the event handler that will be used by the market.
     */
    explicit Market(std::unique_ptr<EventHandler> event_handler);

    /**
     * Adds a new symbol and a corresponding orderbook to market.
     *
     * @param symbol_id the ID that the symbol is identified by, require that
     *                  the symbol associated with symbol ID does not already exist.
     * @param symbol_name the name of the symbol.
     */
    void addSymbol(uint32_t symbol_id, const std::string &symbol_name);

    /**
     * Removes the symbol and the corresponding orderbook from the market.
     *
     * @param symbol_id the ID that the symbol is identified by, require that
     *                  the symbol associated with symbol ID exists.
     */
    void deleteSymbol(uint32_t symbol_id);

    /**
     * @param symbol_id the ID of the symbol to check for the presence of.
     * @return true if the market has the symbol and false otherwise.
     */
    [[nodiscard]] bool hasSymbol(uint32_t symbol_id) const;

    /**
     * Submits a new order to the market.
     *
     * @param order the order to submit.
     */
    void addOrder(const Order &order);

    /**
     * Deletes an existing order from the market, require that the order exists.
     *
     * @param symbol_id the symbol ID associated with the order.
     * @param order_id the ID associated with the order.
     */
    void deleteOrder(uint32_t symbol_id, uint64_t order_id);

    /**
     * Cancels the provided quantity of an existing order in the market, require that the order exists.
     *
     * @param symbol_id the symbol ID associated with the order.
     * @param order_id the ID associated with the order.
     * @param cancelled_quantity the quantity of the order to cancel, require that cancelled_quantity is positive.
     */
    void cancelOrder(uint32_t symbol_id, uint64_t order_id, uint64_t cancelled_quantity);

    /**
     * Replaces an existing order in the market, require that the order exists.
     *
     * @param symbol_id the symbol ID associated with the order.
     * @param order_id the ID associated with the order.
     * @param new_order_id the new ID to assign to the order.
     * @param new_price the new price to assign to the order, require that price is positive.
     */
    void replaceOrder(uint32_t symbol_id, uint64_t order_id, uint64_t new_order_id, uint64_t new_price);

    /**
     * Executes an existing order in the market.
     *
     * @param symbol_id the symbol ID associated with the order.
     * @param order_id the ID associated with the order.
     * @param quantity the quantity of the order to execute, require that quantity is positive.
     * @param price the price at which the order is executed, require that price is positive.
     */
    void executeOrder(uint32_t symbol_id, uint64_t order_id, uint64_t quantity, uint64_t price);

    /**
     * Executes an existing order in the market.
     *
     * @param symbol_id the symbol ID associated with the order.
     * @param order_id the ID associated with the order.
     * @param quantity the quantity of the order to execute, require that quantity is positive.
     */
    void executeOrder(uint32_t symbol_id, uint64_t order_id, uint64_t quantity);

    /**
     * @return the string representation of the market.
     */
    [[nodiscard]] std::string toString() const;

    /**
     * Writes the string representation to a file with the provided
     * name. Creates a new file.
     *
     * @param name the name to the file that will be written to.
     */
    void dumpMarket(const std::string &name) const;

    friend std::ostream &operator<<(std::ostream &os, const Market &book);

private:
    // Submits order operations to their respective orderbook.
    std::unique_ptr<OrderBookHandler> orderbook_handler;
    // Symbol IDs to symbols.
    robin_hood::unordered_map<uint32_t, std::unique_ptr<Symbol>> id_to_symbol;
};
} // namespace RapidTrader
#endif // RAPID_TRADER_MARKET_H
