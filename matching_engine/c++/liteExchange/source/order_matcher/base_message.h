#ifndef _BASE_MESSAGE_H_
#define _BASE_MESSAGE_H_

#include <cstddef>
#include <string>
#include "order.h"
#include <core/memory/heap_memory_cache_aligned_allocators.hpp>

namespace order_matcher
{

class BaseMessage : public core::Aligned<>
{
    public :
        BaseMessage(){}
        explicit BaseMessage(Order order) : m_order(order){}
        virtual ~BaseMessage() {};
        virtual std::string toString() const = 0;
        Order& getOrder() { return m_order; }

    protected :
        Order m_order;
};

}

#endif