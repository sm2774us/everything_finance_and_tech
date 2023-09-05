#ifndef RAPID_TRADER_MAP_ORDERBOOK_H
#define RAPID_TRADER_MAP_ORDERBOOK_H
#include <map>
#include <limits>
#include "robin_hood.h"
#include "level.h"
#include "orderbook.h"
#include "order.h"
#include "event_handler/event_handler.h"

namespace RapidTrader {
// Only validate orderbook in debug mode.
#ifndef NDEBUG
#    define VALIDATE_ORDERBOOK validateOrderBook()
#else
#    define VALIDATE_ORDERBOOK
#endif

struct OrderWrapper
{
    Order order;
    // An iterator to the level that the order is stored in.
    // Used for finding the level to delete the order from in constant time.
    // Be careful about iterator invalidation! For the STL map, this iterator
    // will remain valid as long as element that iterator corresponds to in
    // the map is deleted. Insertions and deletions do not invalidate the iterator.
    std::map<uint64_t, Level>::iterator level_it;
};

class MapOrderBook : public OrderBook
{
public:
    /**
     * A constructor for the MapOrderBook.
     *
     * @param symbol_id_ the symbol ID that will be associated with the book.
     * @param event_handler_ handles updates from the book.
     */
    MapOrderBook(uint32_t symbol_id_, EventHandler &event_handler_);

    /**
     * @inheritdoc
     */
    void addOrder(Order order) override;

    /**
     * @inheritdoc
     */
    void executeOrder(uint64_t order_id, uint64_t quantity, uint64_t price) override;

    /**
     * @inheritdoc
     */
    void executeOrder(uint64_t order_id, uint64_t quantity) override;

    /**
     * @inheritdoc
     */
    void deleteOrder(uint64_t order_id) override;

    /**
     * @inheritdoc
     */
    void cancelOrder(uint64_t order_id, uint64_t quantity) override;

    /**
     * @inheritdoc
     */
    void replaceOrder(uint64_t order_id, uint64_t new_order_id, uint64_t new_price) override;

    /**
     * @inheritdoc
     */
    [[nodiscard]] bool hasOrder(uint64_t order_id) const override
    {
        return orders.find(order_id) != orders.end();
    }

    /**
     * @inheritdoc
     */
    [[nodiscard]] const Order &getOrder(uint64_t order_id) const override
    {
        return orders.find(order_id)->second.order;
    }

    /**
     * @inheritdoc
     */
    [[nodiscard]] bool empty() const override
    {
        return orders.empty();
    }

    /**
     * @inheritdoc
     */
    [[nodiscard]] uint32_t getSymbolID() const override
    {
        return symbol_id;
    }

    /**
     * @inheritdoc
     */
    [[nodiscard]] uint64_t bestBid() const override
    {
        return bid_levels.empty() ? 0 : bid_levels.rbegin()->first;
    }

    /**
     * @inheritdoc
     */
    [[nodiscard]] uint64_t bestAsk() const override
    {
        return ask_levels.empty() ? std::numeric_limits<uint64_t>::max() : ask_levels.begin()->first;
    }

    /**
     * @inheritdoc
     */
    [[nodiscard]] uint64_t lastTradedPrice() const override
    {
        return last_traded_price;
    }

    /**
     * @inheritdoc
     */
    void dumpBook(const std::string &path) const override;

    /**
     * @inheritdoc
     */
    [[nodiscard]] std::string toString() const override;

    friend std::ostream &operator<<(std::ostream &os, const MapOrderBook &book);

private:
    /**
     * Deletes an order from the book. Does not match orders.
     *
     * @param order_id the ID of the order, require that there exists
     *                 an order with order_id in the book.
     * @param notification true if a notification should be sent
     *                     indicating that the order was deleted and
     *                     false otherwise.
     */
    void deleteOrder(uint64_t order_id, bool notification);

    /**
     * Submits a limit order to the book.
     *
     * @param order the limit order to add to the book, require that the order
     *              does not already exist in the book.
     */
    void addLimitOrder(Order &order);

    /**
     * Inserts a limit order into the book.
     *
     * @param order the order to insert.
     */
    void insertLimitOrder(const Order &order);

    /**
     * Submits a market order to the book.
     *
     * @param order the market order to add to the book.
     */
    void addMarketOrder(Order &order);

    /**
     * Inserts a trailing restart order into the book.
     *
     * @param order the trailing restart order to insert.
     */
    void insertTrailingStopOrder(const Order &order);

    /**
     * Submits a restart market order to the book.
     *
     * @param order the restart market order to submit to the book.
     */
    void addStopOrder(Order &order);

    /**
     * Inserts a restart order into the book.
     *
     * @param order the restart order to insert.
     */
    void insertStopOrder(const Order &order);

    /**
     * Calculates the stop price of a trailing stop
     * order and sets the order's stop price to that price.
     *
     * @param order the order to set the stop price of, require that
     *              order is a trailing stop ir trailing stop limit order.
     * @return the new stop price of the order.
     */
    uint64_t calculateStopPrice(Order &order);

    /**
     * Updates the restart price of trailing stop orders on the bid side.
     */
    void updateBidStopOrders();

    /**
     * Updates the stop price of trailing restart orders on the ask side.
     */
    void updateAskStopOrders();

    /**
     * Activates stop limit and restart market orders if the last traded
     * price is suitable.
     */
    void activateStopOrders();

    /**
     * Attempts to activate stop, stop limit, trailing restart, and trailing stop limit
     * orders on the bid side.
     *
     * @return true if any restart orders were activated and false otherwise.
     */
    bool activateBidStopOrders();

    /**
     * Attempts to activate restart, stop limit, trailing stop, and trailing stop limit
     * orders on the ask side.
     *
     * @return true if any restart orders were activated and false otherwise.
     */
    bool activateAskStopOrders();

    /**
     * Removes the provided order from the orderbook, converts it into a market
     * or limit order, and submits it for matching.
     *
     * @param order the order to activate, require that order is has a stop, stop limit,
     *              trailing stop, or trailing stop limit order that is presently in the
     *              orderbook.
     */
    void activateStopOrder(Order order);

    /**
     * Matches all crossed orders in the book. Orders that are filled
     * are removed from the book.
     *
     * @param order the order to match.
     */
    void match(Order &order);

    /**
     * Indicates whether an order is able to completely filled
     * or not.
     *
     * @param order an order.
     * @return true if the order can be completely filled and false
     *         otherwise.
     */
    [[nodiscard]] bool canMatchOrder(const Order &order) const;

    /**
     * Matches two orders.
     *
     * @param ask an ask order to execute.
     * @param bid a bid order to execute.
     * @param executing_price price at which orders are executed, require that
     *                        ask price <= executing_price <= bid price.
     */
    void executeOrders(Order &ask, Order &bid, uint64_t executing_price);

    /**
     * @returns the last traded price if any trades have been made and the max
     *          64-bit unsigned integer value otherwise.
     */
    [[nodiscard]] uint64_t lastTradedPriceAsk() const
    {
        return last_traded_price == 0 ? std::numeric_limits<uint64_t>::max() : last_traded_price;
    }

    /**
     * @returns the last traded price if any trades have been made and zero otherwise.
     */
    [[nodiscard]] uint64_t lastTradedPriceBid() const
    {
        return last_traded_price;
    }

    /*
     * Validates the orderbook.
     *
     * @throws Error if orderbook is in invalid state.
     */
    void validateOrderBook() const;

    /**
     * Validates the limit orders in the order book.
     *
     * @throws Error if any of the following are true: the best bid price
     *               meets or exceeds the best ask price, there is a limit
     *               level that contains an order that is not a limit order,
     *               there is an empty limit level, there is a limit level
     *               on the ask side that contains bid orders (or vice versa),
     *               or there is a limit level that has a price that does
     *               not match the key associated with it in the map.
     */
    void validateLimitOrders() const;

    /**
     * Validates the stop orders in the order book.
     *
     * @throws Error if any of the following are true: there is a stop level
     *               on the ask side with a price that meets or exceeds the
     *               last traded price, there is a stop level that contains
     *               an order that is not a stop order, there is an empty stop
     *               level, there is a stop level on the ask side that contains
     *               bid orders (or vide versa), or there is a stop level that
     *               has a price that does not match the key associated with it
     *               in the map.
     */
    void validateStopOrders() const;

    /**
     * Validates the trailing stop orders in the order book.
     *
     * @throws Error if any of the following are true: there is a trailing
     *               stop level on the ask side with a price that meets or
     *               exceeds the last traded price, there is a trailing stop
     *               level that contains an order that is not a trailing stop order,
     *               there is an empty trailing stop level, there is a trailing stop
     *               level on the ask side that contains bid orders (or vide versa),
     *               or there is a trailing stop level that has a price that does
     *               not match the key associated with it in the map.
     */
    void validateTrailingStopOrders() const;

    // IMPORTANT: Note that the declaration of orders MUST be
    // declared before the declaration of the price level vectors.
    // Class members are destroyed in the reverse order of their declaration,
    // so the price level vectors will be destroyed before orders. This is
    // required for the intrusive list.

    // Maps order IDs to order wrappers.
    robin_hood::unordered_map<uint64_t, OrderWrapper> orders;
    // Maps prices to limit levels.
    std::map<uint64_t, Level> ask_levels;
    std::map<uint64_t, Level> bid_levels;
    // Maps prices to stop levels.
    std::map<uint64_t, Level> stop_ask_levels;
    std::map<uint64_t, Level> stop_bid_levels;
    // Maps prices to trailing stop levels.
    std::map<uint64_t, Level> trailing_stop_ask_levels;
    std::map<uint64_t, Level> trailing_stop_bid_levels;
    // Handles any trade events.
    EventHandler &event_handler;
    // The current price of the symbol - based off the price that the
    // symbol was last traded at. Initially zero.
    uint64_t last_traded_price;
    // Tracks increases and decreases in market price.
    uint64_t trailing_bid_price;
    uint64_t trailing_ask_price;
    // The symbol ID associated with the book.
    uint32_t symbol_id;
};
} // namespace RapidTrader
#endif // RAPID_TRADER_MAP_ORDERBOOK_H
