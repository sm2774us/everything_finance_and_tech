#include "fix_receive_cache.h"
#include "fix_constants.h"
#include "fix_parser.h"
#include <cstring>
using namespace std;

namespace fix
{

FixReceiveCache::FixReceiveCache() : m_capacity{ 0 }, m_size{ 0 }
{
}

void FixReceiveCache::initialise(size_t capacity)
{
    m_buffer.reset(new char[capacity]);
    m_capacity = capacity;
}

size_t FixReceiveCache::parse(vector<FixMessage>* messages)
{
    size_t numMessages{ 0 };
    size_t remainingBytesStartIndex{ 0 };
    size_t remainingBytesEndIndex = m_size-1;

    numMessages = FixParser::parseMultipleMessagesFromBuffer(m_buffer.get(), m_size, messages, remainingBytesStartIndex);

    if (numMessages > 0)
    {
        if (remainingBytesEndIndex > remainingBytesStartIndex)
        {
            moveBufferToStart(remainingBytesStartIndex, remainingBytesEndIndex);
        }
        else
        {
            m_size = 0;
        }
    }

    return numMessages;
}

void FixReceiveCache::moveBufferToStart(size_t startIndex, size_t endIndex)
{
    auto newSize = endIndex - startIndex + 1;
    auto bufferBeginning = m_buffer.get();
    auto copyStart = bufferBeginning + startIndex;
    std::memcpy(bufferBeginning, copyStart, newSize);
    m_size = newSize;
}

}