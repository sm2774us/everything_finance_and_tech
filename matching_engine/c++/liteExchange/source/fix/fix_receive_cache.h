#ifndef _FIX_RECEIVE_CACHE_
#define _FIX_RECEIVE_CACHE_

#include <cstddef>
#include <memory>
#include <vector>
#include "fix_message.h"

namespace fix
{

class FixReceiveCache
{
    public:
        FixReceiveCache();

        void initialise(std::size_t capacity);
        std::size_t parse(std::vector<FixMessage>*);

        char* getBuffer() { return m_buffer.get() + m_size; }
        std::size_t getRemainingBufferSize() const { return m_capacity - m_size; }
        std::size_t getCapacity() const { return m_capacity; }
        void incrementSize(std::size_t size) { m_size += size; }
    private:
        std::size_t m_capacity;
        std::size_t m_size;
        std::unique_ptr<char> m_buffer;
        void moveBufferToStart(std::size_t, std::size_t);
};

}

#endif