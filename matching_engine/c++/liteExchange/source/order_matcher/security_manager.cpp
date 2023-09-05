#include "security_manager.h"
#include <algorithm>
#include <core/string_utility.h>
using namespace std;

namespace order_matcher
{

size_t SecurityManager::addSecurity(const string& symbol)
{
    size_t keyCount = m_securitiesById.size();
    m_securitiesById.insert(std::make_pair(keyCount, symbol));
    m_securitiesBySymbol.insert(std::make_pair(symbol, keyCount));
    return keyCount;
}

size_t SecurityManager::getSecurityId(const string& symbol)const
{
    return m_securitiesBySymbol.at(symbol);
}

const string SecurityManager::getSecurityName(size_t securityId) const
{
    return m_securitiesById.at(securityId);
}

bool SecurityManager::isSecuritySupported(const string& symbol) const
{
    auto element = m_securitiesBySymbol.find(symbol);
    if (element == m_securitiesBySymbol.end())
    {
        return false;
    }
    return true;
}

bool SecurityManager::isSecuritySupported(size_t securityId) const
{
    if (securityId >= 0 && securityId < m_securitiesById.size())
    {
        return true;
    }
    return false;
}


} // namespace