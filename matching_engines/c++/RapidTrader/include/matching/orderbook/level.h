#ifndef RAPID_TRADER_LEVEL_H
#define RAPID_TRADER_LEVEL_H
#include "order.h"

namespace RapidTrader {
// Only validate level in debug mode.
#ifndef NDEBUG
#    define VALIDATE_LEVEL validateLevel()
#else
#    define VALIDATE_LEVEL
#endif

enum class LevelSide
{
    Bid,
    Ask
};

using namespace boost::intrusive;

/**
 * Represents a price level in an orderbook.
 */
class Level
{
public:
    /**
     * A constructor for the level.
     *
     * @param price_ the price associated with the level, require that price_ is positive.
     * @param side_ the side the level is on - either ask or bid.
     * @param symbol_id_ the symbol ID associated with the level.
     */
    Level(uint64_t price_, LevelSide side_, uint32_t symbol_id_);

    /**
     * @return the orders in the level.
     */
    [[nodiscard]] const list<Order> &getOrders() const;

    /**
     * @return the orders in the level.
     */
    [[nodiscard]] list<Order> &getOrders();

    /**
     * @return the price associated with the level.
     */
    [[nodiscard]] uint64_t getPrice() const
    {
        return price;
    }

    /**
     * @return the total volume of the level.
     */
    [[nodiscard]] uint64_t getVolume() const
    {
        return volume;
    }

    /**
     * @return the side of the level - bid or ask.
     */
    [[nodiscard]] LevelSide getSide() const
    {
        return side;
    }

    /**
     * @return the symbol ID associated with the level.
     */
    [[nodiscard]] uint32_t getSymbolID() const
    {
        return symbol_id;
    }

    /**
     * @return true if the level is on the ask side and false otherwise.
     */
    [[nodiscard]] bool isAsk() const
    {
        return side == LevelSide::Ask;
    }

    /**
     * @return true if the order is on the bid side and false otherwise.
     */
    [[nodiscard]] bool isBid() const
    {
        return side == LevelSide::Bid;
    }

    /**
     * @return the number of orders in the level.
     */
    [[nodiscard]] size_t size() const
    {
        return orders.size();
    }

    /**
     * @return true if level is empty and false otherwise.
     */
    [[nodiscard]] bool empty() const
    {
        return orders.empty();
    }

    /**
     * @return the least recently inserted order in the level,
     *         require that the level is non-empty.
     */
    Order &front();

    /**
     * @return the most recently inserted order in the level,
     *         require that the level is non-empty.
     */
    Order &back();

    /**
     * Adds an order to the level.
     *
     * @param order the order to add, require that order is on the same side as the level,
     *              has the same price as the level, and has the same symbol ID as the level.
     */
    void addOrder(Order &order);

    /**
     * Removes the least recently inserted order from the level, require
     * that the level is non-empty.
     */
    void popFront();

    /**
     * Removes the most recently inserted order from the level, require that
     * the level is non-empty.
     */
    void popBack();

    /**
     * Deletes the order from the level.
     *
     * @param order the order to delete, require that the order
     *              is in the level.
     */
    void deleteOrder(const Order &order);

    /**
     * Reduce the current volume of the level.
     *
     * @param amount the amount to reduce the volume by, require that 0 < amount <= volume.
     */
    void reduceVolume(uint64_t amount);

    /**
     * @return the string representation of the level.
     */
    [[nodiscard]] std::string toString() const;

    friend std::ostream &operator<<(std::ostream &os, const Level &level);

private:
    /**
     * Validates the state of the level.
     *
     * @throws Error if any of the following are true: an order
     *               is on a different side than the level, the sum
     *               of the open quantities of the orders in the level
     *               does not equal the volume of the level, the level
     *               contains an order that is a market order,
     *               or the price / stop price of an order does not equal
     *               the price of the level.
     */
    void validateLevel() const;

    list<Order> orders;
    LevelSide side;
    uint32_t symbol_id;
    uint64_t volume;
    uint64_t price;
};
} // namespace RapidTrader
#endif // RAPID_TRADER_LEVEL_H
