#include <iostream>
#include <fstream>
#include "map_orderbook.h"
#include "event_handler/event.h"
#include "log.h"

namespace RapidTrader {
MapOrderBook::MapOrderBook(uint32_t symbol_id_, EventHandler &event_handler_)
    : symbol_id(symbol_id_)
    , event_handler(event_handler_)
    , last_traded_price(0)
    , trailing_bid_price(0)
    , trailing_ask_price(std::numeric_limits<uint64_t>::max())
{}

void MapOrderBook::addOrder(Order order)
{
    event_handler.handleOrderAdded(OrderAdded{order});
    switch (order.getType())
    {
    case OrderType::Limit:
        addLimitOrder(order);
        break;
    case OrderType::Market:
        addMarketOrder(order);
        break;
    case OrderType::Stop:
    case OrderType::StopLimit:
    case OrderType::TrailingStop:
    case OrderType::TrailingStopLimit:
        addStopOrder(order);
        break;
    }
    activateStopOrders();
    VALIDATE_ORDERBOOK;
}

void MapOrderBook::executeOrder(uint64_t order_id, uint64_t quantity, uint64_t price)
{
    auto orders_it = orders.find(order_id);
    Level &executing_level = orders_it->second.level_it->second;
    Order &executing_order = orders_it->second.order;
    uint64_t executing_quantity = std::min(quantity, executing_order.getOpenQuantity());
    executing_order.execute(price, executing_quantity);
    last_traded_price = price;
    event_handler.handleOrderExecuted(ExecutedOrder{executing_order});
    executing_level.reduceVolume(executing_order.getLastExecutedQuantity());
    if (executing_order.isFilled())
        deleteOrder(order_id, true);
    activateStopOrders();
    VALIDATE_ORDERBOOK;
}

void MapOrderBook::executeOrder(uint64_t order_id, uint64_t quantity)
{
    auto orders_it = orders.find(order_id);
    Level &executing_level = orders_it->second.level_it->second;
    Order &executing_order = orders_it->second.order;
    uint64_t executing_quantity = std::min(quantity, executing_order.getOpenQuantity());
    uint64_t executing_price = executing_order.getPrice();
    executing_order.execute(executing_price, executing_quantity);
    last_traded_price = executing_price;
    event_handler.handleOrderExecuted(ExecutedOrder{executing_order});
    executing_level.reduceVolume(executing_order.getLastExecutedQuantity());
    if (executing_order.isFilled())
        deleteOrder(order_id, true);
    activateStopOrders();
    VALIDATE_ORDERBOOK;
}

void MapOrderBook::cancelOrder(uint64_t order_id, uint64_t quantity)
{
    auto orders_it = orders.find(order_id);
    Level &cancelling_level = orders_it->second.level_it->second;
    Order &cancelling_order = orders_it->second.order;
    uint64_t pre_cancellation_quantity = cancelling_order.getOpenQuantity();
    cancelling_order.setQuantity(quantity);
    event_handler.handleOrderUpdated(OrderUpdated{cancelling_order});
    cancelling_level.reduceVolume(pre_cancellation_quantity - cancelling_order.getOpenQuantity());
    if (cancelling_order.isFilled())
        deleteOrder(order_id, true);
    activateStopOrders();
    VALIDATE_ORDERBOOK;
}

void MapOrderBook::deleteOrder(uint64_t order_id)
{
    deleteOrder(order_id, true);
    activateStopOrders();
    VALIDATE_ORDERBOOK;
}

void MapOrderBook::deleteOrder(uint64_t order_id, bool notification)
{
    auto orders_it = orders.find(order_id);
    auto &levels_it = orders_it->second.level_it;
    Order &deleting_order = orders_it->second.order;
    if (notification)
        event_handler.handleOrderDeleted(OrderDeleted{orders_it->second.order});
    levels_it->second.deleteOrder(deleting_order);
    if (levels_it->second.empty())
    {
        switch (deleting_order.getType())
        {
        case OrderType::Limit:
            deleting_order.isAsk() ? ask_levels.erase(levels_it) : bid_levels.erase(levels_it);
            break;
        case OrderType::Stop:
        case OrderType::StopLimit:
            deleting_order.isAsk() ? stop_ask_levels.erase(levels_it) : stop_bid_levels.erase(levels_it);
            break;
        case OrderType::TrailingStop:
        case OrderType::TrailingStopLimit:
            deleting_order.isAsk() ? trailing_stop_ask_levels.erase(levels_it) : trailing_stop_bid_levels.erase(levels_it);
            break;
        default:
            assert(false && "Invalid order type!");
        }
    }
    orders.erase(orders_it);
}

void MapOrderBook::replaceOrder(uint64_t order_id, uint64_t new_order_id, uint64_t new_price)
{
    auto orders_it = orders.find(order_id);
    Order new_order = orders_it->second.order;
    new_order.setOrderID(new_order_id);
    new_order.setPrice(new_price);
    deleteOrder(order_id, true);
    addOrder(new_order);
    activateStopOrders();
    VALIDATE_ORDERBOOK;
}

void MapOrderBook::addLimitOrder(Order &order)
{

    match(order);
    if (!order.isFilled() && !order.isIoc() && !order.isFok())
        insertLimitOrder(order);
    else
        event_handler.handleOrderDeleted(OrderDeleted{order});
}

void MapOrderBook::insertLimitOrder(const Order &order)
{
    if (order.isAsk())
    {
        auto level_it = ask_levels.emplace_hint(ask_levels.begin(), std::piecewise_construct, std::make_tuple(order.getPrice()),
            std::make_tuple(order.getPrice(), LevelSide::Ask, symbol_id));
        auto [orders_it, success] = orders.emplace(order.getOrderID(), OrderWrapper{order, level_it});
        level_it->second.addOrder(orders_it->second.order);
    }
    else
    {
        auto level_it = bid_levels.emplace_hint(bid_levels.end(), std::piecewise_construct, std::make_tuple(order.getPrice()),
            std::make_tuple(order.getPrice(), LevelSide::Bid, symbol_id));
        auto [orders_it, success] = orders.emplace(order.getOrderID(), OrderWrapper{order, level_it});
        level_it->second.addOrder(orders_it->second.order);
    }
}

void MapOrderBook::addMarketOrder(Order &order)
{
    order.setPrice(order.isAsk() ? 0 : std::numeric_limits<uint64_t>::max());
    match(order);
    event_handler.handleOrderDeleted(OrderDeleted{order});
}

void MapOrderBook::addStopOrder(Order &order)
{
    if (order.isTrailingStop() || order.isTrailingStopLimit())
        calculateStopPrice(order);
    uint64_t market_price = order.isAsk() ? lastTradedPriceBid() : lastTradedPriceAsk();
    uint64_t order_stop_price = order.getStopPrice();
    uint8_t match = (order.isAsk() && market_price <= order_stop_price) + (order.isBid() && market_price >= order_stop_price);
    if (match)
    {
        order.setType((order.isStop() || order.isTrailingStop()) ? OrderType::Market : OrderType::Limit);
        order.setStopPrice(0);
        order.setTrailAmount(0);
        event_handler.handleOrderUpdated(OrderUpdated{order});
        order.isMarket() ? addMarketOrder(order) : addLimitOrder(order);
        return;
    }
    order.isTrailingStop() || order.isTrailingStopLimit() ? insertTrailingStopOrder(order) : insertStopOrder(order);
}

void MapOrderBook::insertStopOrder(const Order &order)
{
    if (order.isAsk())
    {
        auto level_it = stop_ask_levels
                            .emplace(std::piecewise_construct, std::make_tuple(order.getStopPrice()),
                                std::make_tuple(order.getStopPrice(), LevelSide::Ask, symbol_id))
                            .first;
        auto [orders_it, success] = orders.emplace(order.getOrderID(), OrderWrapper{order, level_it});
        level_it->second.addOrder(orders_it->second.order);
    }
    else
    {
        auto level_it = stop_bid_levels
                            .emplace(std::piecewise_construct, std::make_tuple(order.getStopPrice()),
                                std::make_tuple(order.getStopPrice(), LevelSide::Bid, symbol_id))
                            .first;
        auto [orders_it, success] = orders.emplace(order.getOrderID(), OrderWrapper{order, level_it});
        level_it->second.addOrder(orders_it->second.order);
    }
}

void MapOrderBook::insertTrailingStopOrder(const Order &order)
{
    if (order.isAsk())
    {
        auto level_it = trailing_stop_ask_levels
                            .emplace(std::piecewise_construct, std::make_tuple(order.getStopPrice()),
                                std::make_tuple(order.getStopPrice(), LevelSide::Ask, symbol_id))
                            .first;
        auto [orders_it, success] = orders.emplace(order.getOrderID(), OrderWrapper{order, level_it});
        level_it->second.addOrder(orders_it->second.order);
        orders_it->second.level_it = level_it;
    }
    else
    {
        auto level_it = trailing_stop_bid_levels
                            .emplace(std::piecewise_construct, std::make_tuple(order.getStopPrice()),
                                std::make_tuple(order.getStopPrice(), LevelSide::Bid, symbol_id))
                            .first;
        auto [orders_it, success] = orders.emplace(order.getOrderID(), OrderWrapper{order, level_it});
        level_it->second.addOrder(orders_it->second.order);
        orders_it->second.level_it = level_it;
    }
}

uint64_t MapOrderBook::calculateStopPrice(Order &order)
{
    if (order.isAsk())
    {
        uint64_t market_price = lastTradedPriceBid();
        uint64_t trail_amount = order.getTrailAmount();
        // Set the new stop price to zero if the trail amount meets or exceeds the market price.
        uint64_t new_stop_price = market_price > trail_amount ? market_price - trail_amount : 0;
        order.setStopPrice(new_stop_price);
        return new_stop_price;
    }
    else
    {
        uint64_t market_price = lastTradedPriceAsk();
        uint64_t trail_amount = order.getTrailAmount();
        // Set the new stop price to max 64-bit integer value if the sum of trail amount and the market
        // price exceeds the max 64-bit integer value.
        uint64_t new_stop_price = market_price < (std::numeric_limits<uint64_t>::max() - trail_amount)
                                      ? market_price + trail_amount
                                      : std::numeric_limits<uint64_t>::max();
        order.setStopPrice(new_stop_price);
        return new_stop_price;
    }
}

void MapOrderBook::activateStopOrders()
{
    bool activate = true;
    // Activating stop orders may result in trades which may result in more stop orders being activated.
    // Continue activating restart orders until there are none left or none can be activated.
    while (activate)
    {
        activate = activateBidStopOrders();
        updateAskStopOrders();
        activate = activateAskStopOrders() || activate;
        updateBidStopOrders();
    }
}

bool MapOrderBook::activateBidStopOrders()
{
    bool activated_orders = false;
    auto stop_levels_it = stop_bid_levels.begin();
    uint64_t last_ask_price = lastTradedPriceAsk();
    while (stop_levels_it != stop_bid_levels.end() && stop_levels_it->first <= last_ask_price)
    {
        activated_orders = true;
        Order &stop_order = stop_levels_it->second.front();
        activateStopOrder(stop_order);
        stop_levels_it = stop_bid_levels.begin();
    }
    auto trailing_stop_levels_it = trailing_stop_bid_levels.begin();
    last_ask_price = lastTradedPriceAsk();
    while (trailing_stop_levels_it != trailing_stop_bid_levels.end() && trailing_stop_levels_it->first <= last_ask_price)
    {
        activated_orders = true;
        Order &trailing_stop_order = trailing_stop_levels_it->second.front();
        activateStopOrder(trailing_stop_order);
        trailing_stop_levels_it = trailing_stop_bid_levels.begin();
    }
    return activated_orders;
}

bool MapOrderBook::activateAskStopOrders()
{
    bool activated_orders = false;
    uint64_t last_bid_price = lastTradedPriceBid();
    auto stop_levels_it = stop_ask_levels.rbegin();
    while (stop_levels_it != stop_ask_levels.rend() && stop_levels_it->first >= last_bid_price)
    {
        activated_orders = true;
        Order &stop_order = stop_levels_it->second.front();
        activateStopOrder(stop_order);
        stop_levels_it = stop_ask_levels.rbegin();
    }
    auto trailing_stop_levels_it = trailing_stop_ask_levels.rbegin();
    last_bid_price = lastTradedPriceBid();
    while (trailing_stop_levels_it != trailing_stop_ask_levels.rend() && trailing_stop_levels_it->first >= last_bid_price)
    {
        activated_orders = true;
        Order &trailing_stop_order = trailing_stop_levels_it->second.front();
        activateStopOrder(trailing_stop_order);
        trailing_stop_levels_it = trailing_stop_ask_levels.rbegin();
    }
    return activated_orders;
}

void MapOrderBook::activateStopOrder(Order order)
{
    deleteOrder(order.getOrderID(), false);
    order.setStopPrice(0);
    order.setTrailAmount(0);
    // Convert the stop / trailing stop order to a limit or market order and match it.
    if (order.isStop() || order.isTrailingStop())
    {
        order.setType(OrderType::Market);
        event_handler.handleOrderUpdated(OrderUpdated{order});
        addMarketOrder(order);
    }
    else
    {
        order.setType(OrderType::Limit);
        event_handler.handleOrderUpdated(OrderUpdated{order});
        addLimitOrder(order);
    }
}

void MapOrderBook::updateBidStopOrders()
{
    if (trailing_ask_price <= lastTradedPriceAsk() || trailing_stop_bid_levels.empty())
    {
        trailing_ask_price = last_traded_price;
        return;
    }
    std::map<uint64_t, Level> new_trailing_levels;
    auto trailing_levels_it = trailing_stop_bid_levels.begin();
    // Update the stop price of all existing trailing stop orders.
    while (trailing_levels_it != trailing_stop_bid_levels.end())
    {
        while (!trailing_levels_it->second.empty())
        {
            Order &stop_order = trailing_levels_it->second.front();
            uint64_t new_stop_price = calculateStopPrice(stop_order);
            auto new_trailing_levels_it = new_trailing_levels.emplace_hint(new_trailing_levels.begin(), std::piecewise_construct,
                std::make_tuple(new_stop_price), std::make_tuple(new_stop_price, LevelSide::Bid, symbol_id));
            orders.find(stop_order.getOrderID())->second.level_it = new_trailing_levels_it;
            trailing_levels_it->second.popFront();
            new_trailing_levels_it->second.addOrder(stop_order);
            event_handler.handleOrderUpdated(OrderUpdated{stop_order});
        }
        ++trailing_levels_it;
    }
    std::swap(trailing_stop_bid_levels, new_trailing_levels);
    trailing_ask_price = last_traded_price;
}

void MapOrderBook::updateAskStopOrders()
{
    if (trailing_bid_price >= lastTradedPriceBid() || trailing_stop_ask_levels.empty())
    {
        trailing_bid_price = last_traded_price;
        return;
    }
    std::map<uint64_t, Level> new_trailing_levels;
    auto trailing_levels_it = trailing_stop_ask_levels.begin();
    // Update the stop price of all existing trailing stop orders.
    while (trailing_levels_it != trailing_stop_ask_levels.end())
    {
        while (!trailing_levels_it->second.empty())
        {
            Order &stop_order = trailing_levels_it->second.front();
            uint64_t new_stop_price = calculateStopPrice(stop_order);
            auto new_trailing_levels_it = new_trailing_levels.emplace_hint(new_trailing_levels.end(), std::piecewise_construct,
                std::make_tuple(new_stop_price), std::make_tuple(new_stop_price, LevelSide::Ask, symbol_id));
            orders.find(stop_order.getOrderID())->second.level_it = new_trailing_levels_it;
            trailing_levels_it->second.popFront();
            new_trailing_levels_it->second.addOrder(stop_order);
            event_handler.handleOrderUpdated(OrderUpdated{stop_order});
        }
        ++trailing_levels_it;
    }
    std::swap(trailing_stop_ask_levels, new_trailing_levels);
    trailing_bid_price = last_traded_price;
}

void MapOrderBook::match(Order &order)
{
    // Order is a FOK order that cannot be filled.
    if (order.isFok() && !canMatchOrder(order))
        return;
    if (order.isAsk())
    {
        auto bid_levels_it = bid_levels.rbegin();
        Order &ask_order = order;
        while (bid_levels_it != bid_levels.rend() && bid_levels_it->first >= ask_order.getPrice() && !ask_order.isFilled())
        {
            Level &bid_level = bid_levels_it->second;
            Order &bid_order = bid_level.front();
            uint64_t executing_price = bid_order.getPrice();
            executeOrders(ask_order, bid_order, executing_price);
            bid_level.reduceVolume(bid_order.getLastExecutedQuantity());
            if (bid_order.isFilled())
                deleteOrder(bid_order.getOrderID(), true);
            // Reset the level iterator - iterator may be invalidated if the order is deleted.
            bid_levels_it = bid_levels.rbegin();
        }
    }
    if (order.isBid())
    {
        auto ask_levels_it = ask_levels.begin();
        Order &bid_order = order;
        while (ask_levels_it != ask_levels.end() && ask_levels_it->first <= bid_order.getPrice() && !bid_order.isFilled())
        {
            Level &ask_level = ask_levels_it->second;
            Order &ask_order = ask_level.front();
            uint64_t executing_price = ask_order.getPrice();
            executeOrders(ask_order, bid_order, executing_price);
            ask_level.reduceVolume(ask_order.getLastExecutedQuantity());
            if (ask_order.isFilled())
                deleteOrder(ask_order.getOrderID(), true);
            // Reset the level iterator - iterator may be invalidated if the order is deleted.
            ask_levels_it = ask_levels.begin();
        }
    }
}

void MapOrderBook::executeOrders(Order &ask, Order &bid, uint64_t executing_price)
{
    // Calculate the minimum quantity to match.
    uint64_t matched_quantity = std::min(ask.getOpenQuantity(), bid.getOpenQuantity());
    bid.execute(executing_price, matched_quantity);
    ask.execute(executing_price, matched_quantity);
    event_handler.handleOrderExecuted(ExecutedOrder{bid});
    event_handler.handleOrderExecuted(ExecutedOrder{ask});
    last_traded_price = executing_price;
}

bool MapOrderBook::canMatchOrder(const Order &order) const
{
    uint64_t price = order.getPrice();
    uint64_t quantity_required = order.getOpenQuantity();
    uint64_t quantity_available = 0;
    if (order.isAsk())
    {
        auto bid_levels_it = bid_levels.rbegin();
        while (bid_levels_it != bid_levels.rend() && bid_levels_it->first >= price)
        {
            uint64_t level_volume = bid_levels_it->second.getVolume();
            uint64_t quantity_needed = quantity_required - quantity_available;
            quantity_available += std::min(level_volume, quantity_needed);
            if (quantity_available >= quantity_required)
                return true;
        }
    }
    else
    {
        auto ask_levels_it = ask_levels.begin();
        while (ask_levels_it != ask_levels.end() && ask_levels_it->first <= price)
        {
            uint64_t level_volume = ask_levels_it->second.getVolume();
            uint64_t quantity_needed = quantity_required - quantity_available;
            quantity_available += std::min(level_volume, quantity_needed);
            if (quantity_available >= quantity_required)
                return true;
        }
    }
    return false;
}

// LCOV_EXCL_START
std::string MapOrderBook::toString() const
{
    std::string book_string;
    book_string += "SYMBOL ID : " + std::to_string(symbol_id) + "\n";
    book_string += "LAST TRADED PRICE: " + std::to_string(last_traded_price) + "\n";
    book_string += "BID ORDERS\n";
    for (const auto &[price, level] : bid_levels)
        book_string += level.toString();
    book_string += "ASK ORDERS\n";
    for (const auto &[price, level] : ask_levels)
        book_string += level.toString();
    book_string += "BID STOP ORDERS\n";
    for (const auto &[price, level] : stop_bid_levels)
        book_string += level.toString();
    book_string += "ASK STOP ORDERS\n";
    for (const auto &[price, level] : stop_ask_levels)
        book_string += level.toString();
    book_string += "BID TRAILING STOP ORDERS\n";
    for (const auto &[price, level] : trailing_stop_bid_levels)
        book_string += level.toString();
    book_string += "ASK TRAILING STOP ORDERS\n";
    for (const auto &[price, level] : trailing_stop_ask_levels)
        book_string += level.toString();
    return book_string;
}

void MapOrderBook::dumpBook(const std::string &path) const
{
    std::ofstream file(path);
    file << toString();
    file.close();
}

std::ostream &operator<<(std::ostream &os, const MapOrderBook &book)
{
    os << book.toString();
    return os;
}

void MapOrderBook::validateOrderBook() const
{
    validateLimitOrders();
    validateStopOrders();
    validateTrailingStopOrders();
}

void MapOrderBook::validateLimitOrders() const
{
    uint64_t current_best_ask = ask_levels.empty() ? std::numeric_limits<uint64_t>::max() : ask_levels.begin()->first;
    uint64_t current_best_bid = bid_levels.empty() ? 0 : bid_levels.rbegin()->first;
    assert(current_best_ask > current_best_bid && "Best bid price should never be lower than best ask price!");

    for (const auto &[price, level] : ask_levels)
    {
        assert(!level.empty() && "Empty limit levels should never be in the orderbook!");
        assert(level.getPrice() == price && "Limit level price should have same value as map key!");
        assert(level.getSide() == LevelSide::Ask && "Limit level with bid side cannot be on the ask side of the book!");
        const auto &level_orders = level.getOrders();
        for (const auto &order : level_orders)
        {
            assert(!order.isFilled() && "Limit level should not contain any filled orders!");
            assert(order.getType() == OrderType::Limit && "Limit level contains order that is not a limit order!");
        }
    }

    for (const auto &[price, level] : bid_levels)
    {
        assert(!level.empty() && "Empty limit levels should never be in the orderbook!");
        assert(level.getPrice() == price && "Limit level price should have same value as map key!");
        assert(level.getSide() == LevelSide::Bid && "Limit level with ask side cannot be on the bid side of the book!");
        const auto &level_orders = level.getOrders();
        for (const auto &order : level_orders)
        {
            assert(!order.isFilled() && "Limit level should not contain any filled orders!");
            assert(order.getType() == OrderType::Limit && "Limit level contains order that is not a limit order!");
        }
    }
}

void MapOrderBook::validateStopOrders() const
{
    for (const auto &[price, level] : stop_ask_levels)
    {
        assert(!level.empty() && "Empty stop levels should never be in the orderbook!");
        assert(price < last_traded_price && "Stop level has ask price that meets or exceeds last traded price!");
        assert(level.getPrice() == price && "Stop Level price should have same value as map key!");
        assert(level.getSide() == LevelSide::Ask && "Stop Level on the ask side cannot be on the bid side of the book!");
        const auto &level_orders = level.getOrders();
        for (const auto &order : level_orders)
        {
            assert(!order.isFilled() && "Stop level should not contain any filled orders!");
            assert(order.getType() == OrderType::Stop ||
                   order.getType() == OrderType::StopLimit && "Stop level contains order that is not a stop order!");
        }
    }

    for (const auto &[price, level] : stop_bid_levels)
    {
        assert(!level.empty() && "Empty stop levels should never be in the orderbook!");
        assert(price > last_traded_price && "Stop level has bid price that meets or is below last traded price!");
        assert(level.getPrice() == price && "Level price should have same value as map key!");
        assert(level.getSide() == LevelSide::Bid && "Stop Level on the bid side cannot be on the ask side of the book!");
        const auto &level_orders = level.getOrders();
        for (const auto &order : level_orders)
        {
            assert(!order.isFilled() && "Stop level should not contain any filled orders!");
            assert(order.getType() == OrderType::Stop ||
                   order.getType() == OrderType::StopLimit && "Stop level contains order that is not a stop order!");
        }
    }
}

void MapOrderBook::validateTrailingStopOrders() const
{
    for (const auto &[price, level] : trailing_stop_ask_levels)
    {
        assert(!level.empty() && "Empty trailing stop levels should never be in the orderbook!");
        assert(price < last_traded_price && "Trailing Stop level has ask price that meets or exceeds last traded price!");
        assert(level.getPrice() == price && "Trailing stop Level price should have same value as map key!");
        assert(level.getSide() == LevelSide::Ask && "Trailing stop Level on the ask side cannot be on the bid side of the book!");
        const auto &level_orders = level.getOrders();
        for (const auto &order : level_orders)
        {
            assert(!order.isFilled() && "Trailing stop level should not contain any filled orders!");
            assert(order.getType() == OrderType::TrailingStop ||
                   order.getType() == OrderType::TrailingStopLimit &&
                       "Trailing stop level contains an order that is not a trailing stop order!");
        }
    }

    for (const auto &[price, level] : trailing_stop_bid_levels)
    {
        assert(!level.empty() && "Empty trailing stop levels should never be in the orderbook!");
        assert(price > last_traded_price && "Trailing stop level has bid price that meets or is below last traded price!");
        assert(level.getPrice() == price && "Level price should have same value as map key!");
        assert(level.getSide() == LevelSide::Bid && "Trailing stop Level on the bid side cannot be on the ask side of the book!");
        const auto &level_orders = level.getOrders();
        for (const auto &order : level_orders)
        {
            assert(!order.isFilled() && "Trailing stop level should not contain any filled orders!");
            assert(order.getType() == OrderType::TrailingStop ||
                   order.getType() == OrderType::TrailingStopLimit &&
                       "Trailing stop level contains an order that is not a trailing stop order!");
        }
    }
}
} // namespace RapidTrader
  // LCOV_EXCL_END