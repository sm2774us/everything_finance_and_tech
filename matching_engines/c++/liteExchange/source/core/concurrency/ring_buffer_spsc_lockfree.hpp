#ifndef _RING_BUFFER_SPSC_LOCKFREE_H_
#define _RING_BUFFER_SPSC_LOCKFREE_H_

#include <cstddef>
#include <atomic>
#include <utility>
#include <memory>
#include <core/noncopyable.h>

namespace core
{

// Bounded queue ( ring buffer ) single producer single consumer queue
// Uses atomic operations with acquire-release memory ordering
template<typename T>
class RingBufferSPSCLockFree : public core::NonCopyable
{
    public:

        explicit RingBufferSPSCLockFree(std::size_t capacity) : m_capacity{capacity}
        {
            assert(capacity > 0);
            std::unique_ptr <T, BufferDeleter> buffer(new T[m_capacity]);
            m_buffer = std::move(buffer);
            m_write.store(0);
            m_read.store(0);
        }

        ~RingBufferSPSCLockFree()
        {
        }

        bool tryPush(T val)
        {
            const auto current_tail = m_write.load();
            const auto next_tail = increment(current_tail);

            if (current_tail - m_read.load(std::memory_order_acquire) <= m_capacity -1 )
            {
                m_buffer.get()[current_tail % m_capacity ] = val;
                m_write.store(next_tail , std::memory_order_release);
                return true;
            }

            return false;
        }

        void push(T val)
        {
            while (!tryPush(val));
        }

        bool tryPop(T* element)
        {
            const auto currentHead = m_read.load();

            if (currentHead != m_write.load(std::memory_order_acquire))
            {
                *element = m_buffer.get()[currentHead % m_capacity];
                m_read.store(increment(currentHead) , std::memory_order_release);
                return true;
            }

            return false;
        }

    private:

        int increment(int n)
        {
            return (n + 1);
        }

        std::atomic<int> m_write;
        std::atomic<int> m_read;
        std::size_t m_capacity;

        struct BufferDeleter
        {
            void operator()(T* memory) { delete[] memory; }
        };

        std::unique_ptr<T, BufferDeleter> m_buffer;

        // Move ctor deletion
        RingBufferSPSCLockFree(RingBufferSPSCLockFree&& other) = delete;
        // Move assignment operator deletion
        RingBufferSPSCLockFree& operator=(RingBufferSPSCLockFree&& other) = delete;
};

}//namespace

#endif