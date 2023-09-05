#include <algorithm>
#include <string>
#include "symbol.h"
namespace RapidTrader {
Symbol::Symbol(uint32_t id_, std::string name_)
    : id(id_)
    , name(std::move(name_))
{}

std::ostream &operator<<(std::ostream &os, const Symbol &symbol)
{
    os << "Symbol Name: " << symbol.name << "Symbol ID: " << symbol.id << "\n";
    return os;
}
} // namespace RapidTrader