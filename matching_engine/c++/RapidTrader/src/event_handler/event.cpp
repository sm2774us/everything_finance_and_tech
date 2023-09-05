#include <iostream>
#include "event_handler/event.h"

// LCOV_EXCL_START
namespace RapidTrader {
std::ostream &operator<<(std::ostream &os, const SymbolAdded &notification)
{
    os << "ADDED SYMBOL\n"
       << "Symbol Name: " << notification.name << "\n"
       << "Symbol ID: " << notification.symbol_id << "\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, const SymbolDeleted &notification)
{
    os << "DELETED SYMBOL\n"
       << "Symbol Name: " << notification.name << "\n"
       << "Symbol ID: " << notification.symbol_id << "\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, const OrderAdded &notification)
{
    os << "ADDED ORDER\n" << notification.order;
    return os;
}

std::ostream &operator<<(std::ostream &os, const ExecutedOrder &notification)
{
    os << "EXECUTED ORDER\n" << notification.order;
    return os;
}

std::ostream &operator<<(std::ostream &os, const OrderDeleted &notification)
{
    os << "DELETED ORDER\n" << notification.order;
    return os;
}

std::ostream &operator<<(std::ostream &os, const OrderUpdated &notification)
{
    os << "UPDATED ORDER\n" << notification.order;
    return os;
}
} // namespace RapidTrader
// LCOV_EXCL_STOP
