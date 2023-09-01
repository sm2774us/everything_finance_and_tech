#include "market/market.h"
#include "map_orderbook.h"

namespace RapidTrader {
OrderBookHandler::OrderBookHandler(std::unique_ptr<EventHandler> event_handler_)
    : event_handler(std::move(event_handler_))
{}

void OrderBookHandler::addOrderBook(uint32_t symbol_id, std::string symbol_name)
{
    auto it = id_to_book.find(symbol_id);
    assert(it == id_to_book.end() && "Symbol already exists!");
    id_to_book.insert({symbol_id, std::make_unique<MapOrderBook>(symbol_id, *event_handler)});
    event_handler->handleSymbolAdded(SymbolAdded{symbol_id, std::move(symbol_name)});
}

void OrderBookHandler::deleteOrderBook(uint32_t symbol_id, std::string symbol_name)
{
    auto it = id_to_book.find(symbol_id);
    assert(it != id_to_book.end() && "Symbol does not exist!");
    id_to_book.erase(it);
    event_handler->handleSymbolDeleted(SymbolDeleted{symbol_id, std::move(symbol_name)});
}

void OrderBookHandler::addOrder(const Order &order)
{
    auto it = id_to_book.find(order.getSymbolID());
    assert(it != id_to_book.end() && "Symbol does not exist!");
    OrderBook *book = it->second.get();
    book->addOrder(order);
}

void OrderBookHandler::deleteOrder(uint32_t symbol_id, uint64_t order_id)
{
    auto it = id_to_book.find(symbol_id);
    assert(it != id_to_book.end() && "Symbol does not exist!");
    OrderBook *book = it->second.get();
    book->deleteOrder(order_id);
}

void OrderBookHandler::cancelOrder(uint32_t symbol_id, uint64_t order_id, uint64_t cancelled_quantity)
{
    auto it = id_to_book.find(symbol_id);
    assert(it != id_to_book.end() && "Symbol does not exist!");
    assert(cancelled_quantity > 0 && "Cancelled quantity must be positive!");
    OrderBook *book = it->second.get();
    book->cancelOrder(order_id, cancelled_quantity);
}

void OrderBookHandler::replaceOrder(uint32_t symbol_id, uint64_t order_id, uint64_t new_order_id, uint64_t new_price)
{
    auto it = id_to_book.find(symbol_id);
    assert(it != id_to_book.end() && "Symbol does not exist!");
    assert(new_order_id > 0 && "Order ID must be positive!");
    assert(new_price > 0 && "Price must be positive!");
    OrderBook *book = it->second.get();
    book->replaceOrder(order_id, new_order_id, new_price);
}

void OrderBookHandler::executeOrder(uint32_t symbol_id, uint64_t order_id, uint64_t quantity, uint64_t price)
{
    auto it = id_to_book.find(symbol_id);
    assert(it != id_to_book.end() && "Symbol does not exist!");
    assert(quantity > 0 && "Quantity must be positive!");
    assert(price > 0 && "Price must be positive!");
    OrderBook *book = it->second.get();
    book->executeOrder(order_id, quantity, price);
}
void OrderBookHandler::executeOrder(uint32_t symbol_id, uint64_t order_id, uint64_t quantity)
{
    auto it = id_to_book.find(symbol_id);
    assert(it != id_to_book.end() && "Symbol does not exist!");
    assert(order_id > 0 && "Order ID must be positive!");
    assert(quantity > 0 && "Quantity must be positive!");
    OrderBook *book = it->second.get();
    book->executeOrder(order_id, quantity);
}

std::string OrderBookHandler::toString()
{
    std::string book_handler_string;
    for (const auto &[symbol_id, book_ptr] : id_to_book)
    {
        if (book_ptr)
            book_handler_string += book_ptr->toString() + "\n";
    }
    return book_handler_string;
}

Market::Market(std::unique_ptr<EventHandler> event_handler)
    : orderbook_handler(std::make_unique<OrderBookHandler>(std::move(event_handler)))
{}

void Market::addSymbol(uint32_t symbol_id, const std::string &symbol_name)
{
    id_to_symbol.insert({symbol_id, std::make_unique<Symbol>(symbol_id, symbol_name)});
    orderbook_handler->addOrderBook(symbol_id, symbol_name);
}

void Market::deleteSymbol(uint32_t symbol_id)
{
    auto it = id_to_symbol.find(symbol_id);
    orderbook_handler->deleteOrderBook(symbol_id, it->second->name);
    id_to_symbol.erase(symbol_id);
}

bool Market::hasSymbol(uint32_t symbol_id) const
{
    return id_to_symbol.find(symbol_id) != id_to_symbol.end();
}

void Market::addOrder(const Order &order)
{
    orderbook_handler->addOrder(order);
}

void Market::deleteOrder(uint32_t symbol_id, uint64_t order_id)
{
    orderbook_handler->deleteOrder(symbol_id, order_id);
}

void Market::cancelOrder(uint32_t symbol_id, uint64_t order_id, uint64_t cancelled_quantity)
{
    orderbook_handler->cancelOrder(symbol_id, order_id, cancelled_quantity);
}

void Market::replaceOrder(uint32_t symbol_id, uint64_t order_id, uint64_t new_order_id, uint64_t new_price)
{
    orderbook_handler->replaceOrder(symbol_id, order_id, new_order_id, new_price);
}

void Market::executeOrder(uint32_t symbol_id, uint64_t order_id, uint64_t quantity, uint64_t price)
{
    orderbook_handler->executeOrder(symbol_id, order_id, quantity, price);
}

void Market::executeOrder(uint32_t symbol_id, uint64_t order_id, uint64_t quantity)
{
    orderbook_handler->executeOrder(symbol_id, order_id, quantity);
}

// LCOV_EXCL_START
std::string Market::toString() const
{
    return orderbook_handler->toString();
}

std::ostream &operator<<(std::ostream &os, const Market &market)
{
    os << market.orderbook_handler->toString();
    return os;
}

void Market::dumpMarket(const std::string &name) const
{
    std::ofstream file(name);
    file << *this;
    file.close();
}
// LCOV_EXCL_END
} // namespace RapidTrader