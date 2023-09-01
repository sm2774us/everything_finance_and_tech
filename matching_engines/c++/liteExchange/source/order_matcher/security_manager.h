#ifndef _SECURITY_MANAGER_H_
#define _SECURITY_MANAGER_H_

#include <string>
#include <unordered_map>
#include <cstddef>
#include <core/design_patterns/singleton_dclp.hpp>

namespace order_matcher
{

// NON THREAD SAFE
class SecurityManager : public core::SingletonDCLP<SecurityManager>
{
    public :
        std::size_t addSecurity(const std::string&);
        std::size_t getSecurityId(const std::string&)const;
        const std::string getSecurityName(std::size_t) const;
        bool isSecuritySupported(const std::string&) const;
        bool isSecuritySupported(std::size_t) const;
    private :
        // Using double hash table
        std::unordered_map<std::size_t, std::string> m_securitiesById;
        std::unordered_map<std::string, std::size_t> m_securitiesBySymbol;
};

} // namespace

#endif