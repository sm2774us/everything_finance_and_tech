#ifndef _CENTRAL_ORDER_BOOK_H_
#define _CENTRAL_ORDER_BOOK_H_

#include "order.h"
#include "order_book.h"
#include "outgoing_message.h"

#include <atomic>
#include <vector>
#include <string>
#include <queue>
#include <cstddef>
#include <unordered_map>
#include <memory>

#include <core/noncopyable.h>
#include <core/concurrency/thread.h>
#include <core/concurrency/ring_buffer_spsc_lockfree.hpp>

#include <core/concurrency/thread_pool.h>
#include <core/design_patterns/visitor.hpp>

namespace order_matcher
{

using OutgoingMessageQueue = std::vector<std::unique_ptr<core::RingBufferSPSCLockFree<OutgoingMessage>>>;

class CentralOrderBook : public core::NonCopyable, public core::Visitable<Order>
{
    public:
        CentralOrderBook();

        ~CentralOrderBook()
        {
            m_orderBookThreadPool.shutdown();
        }

        void accept(core::Visitor<Order>& v) override;

        void setSymbols(const std::vector<std::string>& symbols);
        void initialiseMultithreadedMatching(core::ThreadPoolArguments& args);
        void initialiseOutgoingMessageQueues(int numberOfThreads, int outgoingMessageProcessorQueueSize);

        bool isMatchingMultithreaded() const { return m_isMatchingMultithreaded; }

        bool addOrder(Order& order);
        void rejectOrder(Order& order, const std::string& message);
        void cancelOrder(Order& order, const std::string& origClientOrderID);

        OutgoingMessageQueue* getOutgoingMessageQueue() { return &m_outgoingMessages; }

    private:
        bool m_isMatchingMultithreaded;
        std::atomic<int> m_orderNumber;

        std::unordered_map<std::size_t, OrderBook> m_orderBookDictionary; // SecurityId - OrderBook dictionary
        std::unordered_map<std::size_t, int> m_queueIDDictionary; // SecurityId - Queue ID dictionary , queue ids refer to both thread pool worker queues
                                                                  // and outgoing message processor queues

        std::vector<std::size_t> m_securityIds;
        OutgoingMessageQueue m_outgoingMessages;
        core::ThreadPool m_orderBookThreadPool;

        void* taskNewOrder(Order& order);
        void* taskCancelOrder(Order& order, const std::string& origClientOrderID);

        // Move ctor deletion
        CentralOrderBook(CentralOrderBook&& other) = delete;
        // Move assignment operator deletion
        CentralOrderBook& operator=(CentralOrderBook&& other) = delete;
};

} // namespace

#endif