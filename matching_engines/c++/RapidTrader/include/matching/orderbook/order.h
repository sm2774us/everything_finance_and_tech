#ifndef RAPID_TRADER_ORDER_H
#define RAPID_TRADER_ORDER_H
#include <boost/intrusive/list.hpp>

namespace RapidTrader {
// Only validate order in debug mode.
#ifndef NDEBUG
#    define VALIDATE_ORDER validateOrder()
#else
#    define VALIDATE_ORDER
#endif

using namespace boost::intrusive;
class MapOrderBook;
class Level;

/**
 * Supported order types.
 *
 * Limit: an order type that will only buy / sell a security
 * at a specified price or better. For a limit order
 * on the bid side, the order will only be executed
 * at the price of the order or lower. For a limit
 * order on the ask side, the order will only execute
 * at the price of the order or higher.
 *
 * Market: an order type that will buy / sell a security
 * at the best available price. This type of order
 * will be executed immediately, but the price at
 * which the order is executed is not guaranteed.
 * If the there is not enough volume to completely
 * fill the order, it will be filled as much as possible
 * before being deleted.
 *
 * Stop: an order type that will buy / sell a security once the price
 * of the security reaches the stop price. When the stop price
 * is reached, the order becomes a market order.
 *
 * Stop Limit: an order type that will buy / sell a security once the
 * price of the security reaches the stop price. When the stop price
 * is reached, the order becomes a limit order.
 *
 * Trailing Stop: a stop order that does not have a specific stop price.
 * Instead, the stop price is defined as a specific dollar amount (the trail
 * amount) below or above the market price of the security (the trailing stop price).
 *
 * Trailing Stop Limit: a stop limit order that does not have a specific stop price.
 * Instead, the stop price is defined as a specific dollar amount (the trail
 * amount) below or above the market price of the security (the trailing stop price).
 */
enum class OrderType
{
    Limit = 0,
    Market = 1,
    Stop = 2,
    StopLimit = 3,
    TrailingStop = 4,
    TrailingStopLimit = 5
};

/**
 * Supported order time in force.
 *
 * GTC (Good Till Cancel): an order with time in force GTC will remain
 * active until the order is completed or cancelled.
 *
 * FOK (Fill Or Kill): an order with time in force FOK will be executed
 * immediately in its entirety; otherwise, it will be cancelled.
 *
 * IOC (Immediate Or Cancel): an order with time in force IOC will
 * be executed immediately. Any portion of the IOC order that cannot
 * be filled will be cancelled.
 */
enum class OrderTimeInForce
{
    GTC = 0,
    FOK = 1,
    IOC = 2,
};

/**
 * Order sides.
 *
 * Bid: An order on the bid side is an order to buy a security.
 * Ask: An order on the ask side is an order to sell a security.
 */
enum class OrderSide
{
    Bid = 0,
    Ask = 1
};

struct Order : public list_base_hook<>
{
public:
    /**
     * Creates a market order on the ask side.
     *
     * @param order_id the ID of the order, require that order_id is positive.
     * @param symbol_id the symbol ID of the order, require that symbol_id is positive.
     * @param quantity the quantity of the order, require that quantity is positive.
     * @param time_in_force the time in force of the order, require that time_in_force is either
     *                      FOK or IOC.
     * @return a new market order.
     */
    static Order marketAskOrder(uint64_t order_id, uint32_t symbol_id, uint64_t quantity, OrderTimeInForce time_in_force);

    /**
     * Creates a market order on the bid side.
     *
     * @param order_id the ID of the order, require that order_id is positive.
     * @param symbol_id the symbol ID of the order, require that symbol_id is positive.
     * @param quantity the quantity of the order, require that quantity is positive.
     * @param time_in_force the time in force of the order, require that time_in_force is either
     *                      FOK or IOC.
     * @return a new market order.
     */
    static Order marketBidOrder(uint64_t order_id, uint32_t symbol_id, uint64_t quantity, OrderTimeInForce time_in_force);

    /**
     * Creates a new limit order on the ask side.
     *
     * @param order_id the ID of the order, require that order_id is positive.
     * @param symbol_id the symbol ID of the order, require that symbol_id is positive.
     * @param price the price of the order, require that price is positive.
     * @param quantity the quantity of the order, require that quantity is positive.
     * @param time_in_force the time in force of the order.
     * @return a new limit order.
     */
    static Order limitAskOrder(uint64_t order_id, uint32_t symbol_id, uint64_t price, uint64_t quantity, OrderTimeInForce time_in_force);

    /**
     * Creates a new limit order on the bid side.
     *
     * @param order_id the ID of the order, require that order_id is positive.
     * @param symbol_id the symbol ID of the order, require that symbol_id is positive.
     * @param price the price of the order, require that price is positive.
     * @param quantity the quantity of the order, require that quantity is positive.
     * @param time_in_force the time in force of the order.
     * @return a new limit order.
     */
    static Order limitBidOrder(uint64_t order_id, uint32_t symbol_id, uint64_t price, uint64_t quantity, OrderTimeInForce time_in_force);

    /**
     * Creates a new stop market order on the ask side.
     *
     * @param order_id the ID of the order, require that ID is positive.
     * @param symbol_id the symbol ID of the order, require that symbol_id is positive.
     * @param stop_price the stop price of the order, require that stop_price is positive.
     * @param quantity the quantity of the order, require that quantity is positive.
     * @param time_in_force the time in force of the order, require that time_in_force is either
     *                      FOK or IOC.
     * @return a new stop market order.
     */
    static Order stopAskOrder(
        uint64_t order_id, uint32_t symbol_id, uint64_t stop_price, uint64_t quantity, OrderTimeInForce time_in_force);

    /**
     * Creates a new stop market order on the bid side.
     *
     * @param order_id the ID of the order, require that order_id is positive.
     * @param symbol_id the symbol ID of the order, require that symbol_id is positive.
     * @param stop_price the stop price of the order, require that stop_price is positive.
     * @param quantity the quantity of the order, require that quantity is positive.
     * @param time_in_force the time in force of the order, require that time_in_force is either
     *                      FOK or IOC.
     * @return a new stop market order.
     */
    static Order stopBidOrder(
        uint64_t order_id, uint32_t symbol_id, uint64_t stop_price, uint64_t quantity, OrderTimeInForce time_in_force);

    /**
     * Creates a new stop limit order on the ask side.
     *
     * @param order_id the ID of the order, require that order_id is positive.
     * @param symbol_id the symbol ID of the order, require that symbol_id is positive.
     * @param price the price that the order, require that price is positive.
     * @param stop_price the stop price of the order, require that stop_price is positive.
     * @param quantity the quantity of the order, require that quantity is positive.
     * @param time_in_force the time in force of the order.
     * @return a new stop limit order.
     */
    static Order stopLimitAskOrder(
        uint64_t order_id, uint32_t symbol_id, uint64_t price, uint64_t stop_price, uint64_t quantity, OrderTimeInForce time_in_force);

    /**
     * Creates a new stop limit order on the bid side.
     *
     * @param order_id the ID of the order, require that order_id is positive.
     * @param symbol_id the symbol ID of the order, require that symbol_id is positive.
     * @param price the price that the order, require that price is positive.
     * @param stop_price the stop price of the order, require that stop_price is positive.
     * @param quantity the quantity of the order, require that quantity is positive.
     * @param time_in_force the time in force of the order.
     * @return a new stop limit order.
     */
    static Order stopLimitBidOrder(
        uint64_t order_id, uint32_t symbol_id, uint64_t price, uint64_t stop_price, uint64_t quantity, OrderTimeInForce time_in_force);

    /**
     * Create a new trailing stop order on the ask side.
     *
     * @param order_id the ID of the order, require that order_id is positive.
     * @param symbol_id the symbol ID of the order, require that the symbol_id is positive.
     * @param trail_amount the trail amount, require that trail_amount is positive.
     * @param quantity the quantity of the order, require that quantity is positive.
     * @param time_in_force the time in force of the order, require that time in force is IOC or FOK.
     * @return a new trailing stop order.
     */
    static Order trailingStopAskOrder(
        uint64_t order_id, uint32_t symbol_id, uint64_t trail_amount, uint64_t quantity, OrderTimeInForce time_in_force);

    /**
     * Create a new trailing stop order on the bid side.
     *
     * @param order_id the ID of the order, require that order_id is positive.
     * @param symbol_id the symbol ID of the order, require that the symbol_id is positive.
     * @param trail_amount the trail amount, require that trail_amount is positive.
     * @param quantity the quantity of the order, require that quantity is positive.
     * @param time_in_force the time in force of the order, require that time in force is IOC or FOK.
     * @return a new trailing stop order.
     */
    static Order trailingStopBidOrder(
        uint64_t order_id, uint32_t symbol_id, uint64_t trail_amount, uint64_t quantity, OrderTimeInForce time_in_force);

    /**
     * Create a new trailing stop limit order on the ask side.
     *
     * @param order_id the ID of the order, require that order_id is positive.
     * @param symbol_id the symbol ID of the order, require that the symbol_id is positive.
     * @param price the price of the order, require that price is positive.
     * @param trail_amount the trail amount, require that trail_amount is positive.
     * @param quantity the quantity of the order, require that quantity is positive.
     * @param time_in_force the time in force of the order, require that time in force is IOC or FOK.
     * @return a new trailing stop limit order.
     */
    static Order trailingStopLimitAskOrder(
        uint64_t order_id, uint32_t symbol_id, uint64_t price, uint64_t trail_amount, uint64_t quantity, OrderTimeInForce time_in_force);

    /**
     * Create a new trailing stop limit order on the bid side.
     *
     * @param order_id the ID of the order, require that order_id is positive.
     * @param symbol_id the symbol ID of the order, require that the symbol_id is positive.
     * @param price the price of the order, require that price is positive.
     * @param trail_amount the trail amount, require that trail_amount is positive.
     * @param quantity the quantity of the order, require that quantity is positive.
     * @param time_in_force the time in force of the order, require that time in force is IOC or FOK.
     * @return a new trailing stop limit order.
     */
    static Order trailingStopLimitBidOrder(
        uint64_t order_id, uint32_t symbol_id, uint64_t price, uint64_t trail_amount, uint64_t quantity, OrderTimeInForce time_in_force);

    /**
     * @return the quantity of the order.
     */
    [[nodiscard]] uint64_t getQuantity() const
    {
        return quantity;
    }

    /**
     * @return the quantity of the order that has been executed.
     */
    [[nodiscard]] uint64_t getExecutedQuantity() const
    {
        return executed_quantity;
    }

    /**
     * @return the quantity of the order that remains to be executed.
     */
    [[nodiscard]] uint64_t getOpenQuantity() const
    {
        return open_quantity;
    }

    /**
     * @return the last executed quantity of the order if the order
     *         has been executed, otherwise zero.
     */
    [[nodiscard]] uint64_t getLastExecutedQuantity() const
    {
        return last_executed_quantity;
    }

    /**
     * @return the ID associated with the order.
     */
    [[nodiscard]] uint64_t getOrderID() const
    {
        return id;
    }

    /**
     * @return the price associated with the order.
     */
    [[nodiscard]] uint64_t getPrice() const
    {
        return price;
    }

    /**
     * @return the stop price associated with the order
     *         if applicable, otherwise zero.
     */
    [[nodiscard]] uint64_t getStopPrice() const
    {
        return stop_price;
    }

    /**
     * @return the trail amount associated with the order
     *         if applicable, otherwise zero.
     */
    [[nodiscard]] uint64_t getTrailAmount() const
    {
        return trail_amount;
    }

    /**
     * @return the side of the order - ask or bid.
     */
    [[nodiscard]] OrderSide getSide() const
    {
        return side;
    }

    /**
     * @return the type of the order - limit, market, stop, stop limit, trailing stop,
     *         or trailing stop limit.
     */
    [[nodiscard]] OrderType getType() const
    {
        return type;
    }

    /**
     * @return the time in force of the order - FOK, GTC, or IOC.
     */
    [[nodiscard]] OrderTimeInForce getTimeInForce() const
    {
        return time_in_force;
    }

    /**
     * @return the price at which the order was last executed if the order
     *         has been executed, otherwise zero.
     */
    [[nodiscard]] uint64_t getLastExecutedPrice() const
    {
        return last_executed_price;
    }

    /**
     * @return the symbol ID associated with the order.
     */
    [[nodiscard]] uint32_t getSymbolID() const
    {
        return symbol_id;
    }

    /**
     * @return true if order is on the ask side and false otherwise.
     */
    [[nodiscard]] bool isAsk() const
    {
        return side == OrderSide::Ask;
    }

    /**
     * @return true if order is on the bid side and false otherwise.
     */
    [[nodiscard]] bool isBid() const
    {
        return side == OrderSide::Bid;
    }

    /**
     * @return true if the order is a limit order and false otherwise.
     */
    [[nodiscard]] bool isLimit() const
    {
        return type == OrderType::Limit;
    }

    /**
     * @return true if the order is a market order and false otherwise.
     */
    [[nodiscard]] bool isMarket() const
    {
        return type == OrderType::Market;
    }

    /**
     * @return true if the order is a stop order and false otherwise.
     */
    [[nodiscard]] bool isStop() const
    {
        return type == OrderType::Stop;
    }

    /**
     * @return true if the order is a stop limit order and false otherwise.
     */
    [[nodiscard]] bool isStopLimit() const
    {
        return type == OrderType::StopLimit;
    }

    /**
     * @return true if the order is a trailing stop order and false otherwise.
     */
    [[nodiscard]] bool isTrailingStop() const
    {
        return type == OrderType::TrailingStop;
    }

    /**
     * @return true if the order is a trailing stop limit order and false otherwise.
     */
    [[nodiscard]] bool isTrailingStopLimit() const
    {
        return type == OrderType::TrailingStopLimit;
    }

    /**
     * @return true if the order is an IOC order and false otherwise.
     */
    [[nodiscard]] bool isIoc() const
    {
        return time_in_force == OrderTimeInForce::IOC;
    }

    /**
     * @return true if the order is a GTC order and false otherwise.
     */
    [[nodiscard]] bool isGtc() const
    {
        return time_in_force == OrderTimeInForce::GTC;
    }

    /**
     * @return true if the order is a FOK order and false otherwise.
     */
    [[nodiscard]] bool isFok() const
    {
        return time_in_force == OrderTimeInForce::FOK;
    }

    /**
     * @return true if the order is filled (i.e. the entire quantity of the order
     *         has been executed) and false otherwise.
     */
    [[nodiscard]] bool isFilled() const
    {
        return open_quantity == 0;
    }

    /**
     * Indicates whether two orders are equal. Two orders are equal iff they
     * have the same order ID.
     *
     * @param other another order.
     * @return true if the orders are equal and false otherwise.
     */
    bool operator==(const Order &other) const
    {
        return id == other.id;
    }

    /**
     * Indicates whether two orders are not equal.
     *
     * @param other another order.
     * @return true if the orders are equal and false otherwise.
     */
    bool operator!=(const Order &other) const
    {
        return !(*this == other);
    }

    /**
     * @return the string representation of the order.
     */
    [[nodiscard]] std::string toString() const;

    friend std::ostream &operator<<(std::ostream &os, const Order &order);
    friend class MapOrderBook;
    friend class Level;

private:
    /**
     * A constructor for the Order.
     *
     * @param type_ the type of the order - Limit, Market, Stop, Stop Limit, Trailing Stop, or Trailing Stop Limit.
     * @param side_ the side the order is on - ask or bid.
     * @param time_in_force_ the time in force of the order - FOK, GTC, or IOC.
     * @param symbol_id_ the symbol ID associated with the order.
     * @param price_ the price of the order.
     * @param stop_price_ the stop price of the order.
     * @param trail_amount_ the absolute distance from the market price that trailing stop and trailing
     *                      stop limit orders will trail.
     * @param quantity_ the quantity of the order.
     * @param id_ the ID associated with the order.
     */
    Order(OrderType type_, OrderSide side_, OrderTimeInForce time_in_force_, uint32_t symbol_id_, uint64_t price_, uint64_t stop_price_,
        uint64_t trail_amount_, uint64_t quantity_, uint64_t id_);

    /**
     * Executes the order.
     *
     * @param price_ the price at which to execute the order, require that price is positive.
     * @param quantity_ the quantity of the order to execute, require that quantity is positive.
     */
    void execute(uint32_t price_, uint64_t quantity_)
    {
        open_quantity -= quantity_;
        executed_quantity += quantity_;
        last_executed_price = price_;
        last_executed_quantity = quantity_;
        VALIDATE_ORDER;
    }

    /**
     * Set the price of the order.
     *
     * @param price_ the new price of that order, require that price
     *               is positive if the order is not a market order.
     */
    void setPrice(uint64_t price_)
    {
        price = price_;
        VALIDATE_ORDER;
    }

    /**
     * Set the stop price of the order, require that the order is a restart, stop limit,
     * or trailing restart order.
     *
     * @param stop_price_ the new price of that order, require that price_ is positive.
     */
    void setStopPrice(uint64_t stop_price_)
    {
        stop_price = stop_price_;
        VALIDATE_ORDER;
    }

    /**
     * Set the stop trail amount of the order, require that the order is a
     * or trailing stop or trailing stop limit order.
     *
     * @param trail_amount_ the new trail amount of the order.
     */
    void setTrailAmount(uint64_t trail_amount_)
    {
        trail_amount = trail_amount_;
        VALIDATE_ORDER;
    }

    /**
     * Set the quantity of the order.
     *
     * @param quantity_ the new quantity of the order, require that quantity_ is positive.
     */
    void setQuantity(uint64_t quantity_)
    {
        quantity = std::min(quantity_, open_quantity);
        open_quantity -= quantity_;
        VALIDATE_ORDER;
    }

    /**
     * Set the ID of the order.
     *
     * @param id_ the new ID of the order.
     */
    void setOrderID(uint64_t id_)
    {
        id = id_;
        VALIDATE_ORDER;
    }

    /**
     * Set the time_in_force of the order.
     *
     * @param action_ the new time_in_force of the order, require that, if type_ is
     *                market or restart, the time_in_force of the order is FOK or IOC.
     */
    void setType(OrderType action_)
    {
        type = action_;
        VALIDATE_ORDER;
    }

    /**
     * Validates the state of the order.
     *
     * @throws Error if any of the following are true: the order
     *               has a non-positive quantity, the order has a non-positive
     *               ID, the order is a market, stop, or trailing stop order that
     *               does not have time in force FOK or IOC, the order is trailing
     *               stop or trailing stop limit order that does not have a positive
     *               trail amount, the order is a stop or stop limit order that does
     *               not have a positive stop price, or the executed quantity / last
     *               executed quantity of the order exceeds the quantity of the order.
     */
    void validateOrder() const;

    OrderType type;
    OrderSide side;
    OrderTimeInForce time_in_force;
    uint32_t symbol_id;
    uint64_t price;
    uint64_t stop_price;
    uint64_t trail_amount;
    uint64_t last_executed_price;
    uint64_t id;
    uint64_t quantity;
    uint64_t executed_quantity;
    uint64_t open_quantity;
    uint64_t last_executed_quantity;
};
} // namespace RapidTrader
#endif // RAPID_TRADER_ORDER_H
