#ifndef RAPID_TRADER_EVENT_H
#define RAPID_TRADER_EVENT_H
#include "order.h"

namespace RapidTrader {
struct Event
{
    virtual ~Event() = default;
};

struct MarketEvent : public Event
{
    uint32_t symbol_id;
    explicit MarketEvent(uint32_t symbol_id_)
        : symbol_id(symbol_id_)
    {}
};

struct SymbolAdded : public MarketEvent
{
    std::string name;
    SymbolAdded(uint32_t symbol_id_, std::string name_)
        : MarketEvent(symbol_id_)
        , name(std::move(name_))
    {}

    friend std::ostream &operator<<(std::ostream &os, const SymbolAdded &notification);
};

struct SymbolDeleted : public MarketEvent
{
    std::string name;
    SymbolDeleted(uint32_t symbol_id_, std::string name_)
        : MarketEvent(symbol_id_)
        , name(std::move(name_))
    {}

    friend std::ostream &operator<<(std::ostream &os, const SymbolDeleted &notification);
};

struct OrderEvent : public Event
{
    Order order;
    explicit OrderEvent(Order order_)
        : order(std::move(order_))
    {}
};

struct OrderAdded : public OrderEvent
{
    explicit OrderAdded(Order order_)
        : OrderEvent(std::move(order_))
    {}

    friend std::ostream &operator<<(std::ostream &os, const OrderAdded &notification);
};

struct OrderDeleted : public OrderEvent
{
    explicit OrderDeleted(Order order_)
        : OrderEvent(std::move(order_))
    {}

    friend std::ostream &operator<<(std::ostream &os, const OrderDeleted &notification);
};

struct ExecutedOrder : public OrderEvent
{
    explicit ExecutedOrder(Order order_)
        : OrderEvent(std::move(order_))
    {}

    friend std::ostream &operator<<(std::ostream &os, const ExecutedOrder &notification);
};

struct OrderUpdated : public OrderEvent
{
    explicit OrderUpdated(Order order_)
        : OrderEvent(std::move(order_))
    {}

    friend std::ostream &operator<<(std::ostream &os, const OrderUpdated &notification);
};
} // namespace RapidTrader
#endif // RAPID_TRADER_EVENT_H
