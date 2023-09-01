#include <iostream>
#include "order.h"

namespace RapidTrader {
Order::Order(OrderType type_, OrderSide side_, OrderTimeInForce time_in_force_, uint32_t symbol_id_, uint64_t price_, uint64_t stop_price_,
    uint64_t trail_amount_, uint64_t quantity_, uint64_t id_)
    : type(type_)
    , side(side_)
    , time_in_force(time_in_force_)
    , symbol_id(symbol_id_)
    , price(price_)
    , stop_price(stop_price_)
    , trail_amount(trail_amount_)
    , quantity(quantity_)
    , id(id_)
{
    last_executed_price = 0;
    executed_quantity = 0;
    open_quantity = quantity;
    last_executed_price = 0;
    last_executed_quantity = 0;
    VALIDATE_ORDER;
}

Order Order::marketAskOrder(uint64_t order_id, uint32_t symbol_id, uint64_t quantity, OrderTimeInForce time_in_force)
{
    assert(time_in_force != OrderTimeInForce::GTC && "Market orders cannot gave GTC time in force!");
    assert(order_id > 0 && "Order ID must be positive!");
    assert(symbol_id > 0 && "Symbol ID must be positive!");
    assert(quantity > 0 && "Quantity must be positive!");
    return Order{OrderType::Market, OrderSide::Ask, time_in_force, symbol_id, 0, 0, 0, quantity, order_id};
}

Order Order::marketBidOrder(uint64_t order_id, uint32_t symbol_id, uint64_t quantity, OrderTimeInForce time_in_force)
{
    assert(time_in_force != OrderTimeInForce::GTC && "Market orders cannot gave GTC time in force!");
    assert(order_id > 0 && "Order ID must be positive!");
    assert(symbol_id > 0 && "Symbol ID must be positive!");
    assert(quantity > 0 && "Quantity must be positive!");
    return Order{OrderType::Market, OrderSide::Bid, time_in_force, symbol_id, 0, 0, 0, quantity, order_id};
}

Order Order::limitAskOrder(uint64_t order_id, uint32_t symbol_id, uint64_t price, uint64_t quantity, OrderTimeInForce time_in_force)
{
    assert(order_id > 0 && "Order ID must be positive!");
    assert(symbol_id > 0 && "Symbol ID must be positive!");
    assert(price > 0 && "Price must be positive!");
    assert(quantity > 0 && "Quantity must be positive!");
    return Order{OrderType::Limit, OrderSide::Ask, time_in_force, symbol_id, price, 0, 0, quantity, order_id};
}

Order Order::limitBidOrder(uint64_t order_id, uint32_t symbol_id, uint64_t price, uint64_t quantity, OrderTimeInForce time_in_force)
{
    assert(order_id > 0 && "Order ID must be positive!");
    assert(symbol_id > 0 && "Symbol ID must be positive!");
    assert(price > 0 && "Price must be positive!");
    assert(quantity > 0 && "Quantity must be positive!");
    return Order{OrderType::Limit, OrderSide::Bid, time_in_force, symbol_id, price, 0, 0, quantity, order_id};
}

Order Order::stopAskOrder(uint64_t order_id, uint32_t symbol_id, uint64_t stop_price, uint64_t quantity, OrderTimeInForce time_in_force)
{
    assert(time_in_force != OrderTimeInForce::GTC && "Stop orders cannot gave GTC time in force!");
    assert(order_id > 0 && "Order ID must be positive!");
    assert(symbol_id > 0 && "Symbol ID must be positive!");
    assert(stop_price > 0 && "Price must be positive!");
    assert(quantity > 0 && "Quantity must be positive!");
    return Order{OrderType::Stop, OrderSide::Ask, time_in_force, symbol_id, 0, stop_price, 0, quantity, order_id};
}

Order Order::stopBidOrder(uint64_t order_id, uint32_t symbol_id, uint64_t stop_price, uint64_t quantity, OrderTimeInForce time_in_force)
{
    assert(time_in_force != OrderTimeInForce::GTC && "Stop orders cannot gave GTC time in force!");
    assert(order_id > 0 && "Order ID must be positive!");
    assert(symbol_id > 0 && "Symbol ID must be positive!");
    assert(stop_price > 0 && "Stop Price must be positive!");
    assert(quantity > 0 && "Quantity must be positive!");
    return Order{OrderType::Stop, OrderSide::Bid, time_in_force, symbol_id, 0, stop_price, 0, quantity, order_id};
}

Order Order::stopLimitAskOrder(
    uint64_t order_id, uint32_t symbol_id, uint64_t price, uint64_t stop_price, uint64_t quantity, OrderTimeInForce time_in_force)
{
    assert(order_id > 0 && "Order ID must be positive!");
    assert(symbol_id > 0 && "Symbol ID must be positive!");
    assert(price > 0 && "Price must be positive!");
    assert(stop_price > 0 && "Stop Price must be positive!");
    assert(quantity > 0 && "Quantity must be positive!");
    return Order{OrderType::StopLimit, OrderSide::Ask, time_in_force, symbol_id, price, stop_price, 0, quantity, order_id};
}

Order Order::stopLimitBidOrder(
    uint64_t order_id, uint32_t symbol_id, uint64_t price, uint64_t stop_price, uint64_t quantity, OrderTimeInForce time_in_force)
{
    assert(order_id > 0 && "Order ID must be positive!");
    assert(symbol_id > 0 && "Symbol ID must be positive!");
    assert(price > 0 && "Price must be positive!");
    assert(stop_price > 0 && "Stop Price must be positive!");
    assert(quantity > 0 && "Quantity must be positive!");
    return Order{OrderType::StopLimit, OrderSide::Bid, time_in_force, symbol_id, price, stop_price, 0, quantity, order_id};
}

Order Order::trailingStopAskOrder(
    uint64_t order_id, uint32_t symbol_id, uint64_t trail_amount, uint64_t quantity, OrderTimeInForce time_in_force)
{
    assert(time_in_force != OrderTimeInForce::GTC && "Stop orders cannot gave GTC time in force!");
    assert(order_id > 0 && "Order ID must be positive!");
    assert(symbol_id > 0 && "Symbol ID must be positive!");
    assert(trail_amount > 0 && "Stop Price must be positive!");
    assert(quantity > 0 && "Quantity must be positive!");
    return Order{OrderType::TrailingStop, OrderSide::Ask, time_in_force, symbol_id, 0, 0, trail_amount, quantity, order_id};
}

Order Order::trailingStopBidOrder(
    uint64_t order_id, uint32_t symbol_id, uint64_t trail_amount, uint64_t quantity, OrderTimeInForce time_in_force)
{
    assert(time_in_force != OrderTimeInForce::GTC && "Stop orders cannot gave GTC time in force!");
    assert(order_id > 0 && "Order ID must be positive!");
    assert(symbol_id > 0 && "Symbol ID must be positive!");
    assert(trail_amount > 0 && "Stop Price must be positive!");
    assert(quantity > 0 && "Quantity must be positive!");
    return Order{OrderType::TrailingStop, OrderSide::Bid, time_in_force, symbol_id, 0, 0, trail_amount, quantity, order_id};
}

Order Order::trailingStopLimitAskOrder(
    uint64_t order_id, uint32_t symbol_id, uint64_t price, uint64_t trail_amount, uint64_t quantity, OrderTimeInForce time_in_force)
{
    assert(order_id > 0 && "Order ID must be positive!");
    assert(symbol_id > 0 && "Symbol ID must be positive!");
    assert(price > 0 && "Price must be positive!");
    assert(trail_amount > 0 && "Trail amount must be positive!");
    assert(quantity > 0 && "Quantity must be positive!");
    return Order{OrderType::TrailingStopLimit, OrderSide::Ask, time_in_force, symbol_id, price, 0, trail_amount, quantity, order_id};
}

Order Order::trailingStopLimitBidOrder(
    uint64_t order_id, uint32_t symbol_id, uint64_t price, uint64_t trail_amount, uint64_t quantity, OrderTimeInForce time_in_force)
{
    assert(order_id > 0 && "Order ID must be positive!");
    assert(symbol_id > 0 && "Symbol ID must be positive!");
    assert(price > 0 && "Price must be positive!");
    assert(trail_amount > 0 && "Trail amount must be positive!");
    assert(quantity > 0 && "Quantity must be positive!");
    return Order{OrderType::TrailingStopLimit, OrderSide::Bid, time_in_force, symbol_id, price, 0, trail_amount, quantity, order_id};
}

// LCOV_EXCL_START
static std::string typeToString(OrderType type)
{
    switch (type)
    {
    case OrderType::Limit:
        return "LIMIT";
    case OrderType::Market:
        return "MARKET";
    case OrderType::Stop:
        return "STOP";
    case OrderType::StopLimit:
        return "STOP LIMIT";
    case OrderType::TrailingStop:
        return "TRAILING STOP";
    case OrderType::TrailingStopLimit:
        return "TRAILING STOP LIMIT";
    default:
        assert(false && "Invalid order type!");
    }
    return "";
}

static std::string sideToString(OrderSide side)
{
    switch (side)
    {
    case OrderSide::Ask:
        return "ASK";
    case OrderSide::Bid:
        return "BID";
    default:
        assert(false && "Invalid order side!");
    }
    return "";
}

static std::string timeInForceToString(OrderTimeInForce tof)
{
    switch (tof)
    {
    case OrderTimeInForce::GTC:
        return "GTC";
    case OrderTimeInForce::FOK:
        return "FOK";
    case OrderTimeInForce::IOC:
        return "IOC";
    default:
        assert(false && "Invalid order time in force!");
    }
    return "";
}

std::string Order::toString() const
{
    std::string order_string;
    order_string += "Symbol ID: " + std::to_string(symbol_id) + "\n";
    order_string += "Order ID: " + std::to_string(id) + "\n";
    order_string += "Type: " + typeToString(type) + "\n";
    order_string += "Side: " + sideToString(side) + "\n";
    order_string += "TOF: " + timeInForceToString(time_in_force) + "\n";
    order_string += "Price: " + std::to_string(price) + "\n";
    order_string += "Quantity: " + std::to_string(quantity) + "\n";
    order_string += "Open Quantity: " + std::to_string(open_quantity) + "\n";
    return order_string;
}

std::ostream &operator<<(std::ostream &os, const Order &order)
{
    os << order.toString();
    return os;
}

void Order::validateOrder() const
{
    // Price should always be positive for limit, stop limit, and trailing stop limit orders.
    if (type == OrderType::Limit || type == OrderType::StopLimit || type == OrderType::TrailingStopLimit)
        assert(price > 0 && "Limit, stop limit, and trailing stop limit orders must have a positive price!");
    // Market orders, stop orders, and trailing stop orders can only have time in force FOK or IOC.
    if (type == OrderType::Market || type == OrderType::Stop || type == OrderType::TrailingStop)
        assert(time_in_force != OrderTimeInForce::GTC && "Market and stop orders cannot have GTC time in force!");
    // All orders must have positive quantity.
    assert(quantity > 0 && "Orders must have a positive quantity!");
    // Last executed quantity and executed quantity should never exceed the quantity of the order.
    assert(last_executed_quantity <= quantity && "Last executed quantity of the order should never exceed the quantity of the order!");
    assert(executed_quantity <= quantity && "Executed quantity of the order should never exceed the quantity of the order!");
    // All orders must have a positive ID.
    assert(id > 0 && "Order ID must be positive!");
}
} // namespace RapidTrader
// LCOV_EXCL_STOP
