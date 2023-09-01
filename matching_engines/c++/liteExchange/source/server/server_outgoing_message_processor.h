#ifndef _OUTGOING_MESSAGE_PROCESSOR_H_
#define _OUTGOING_MESSAGE_PROCESSOR_H_

#include <cstddef>
#include <string>
#include <memory>
#include <core/concurrency/thread_wait_strategy.h>
#include <core/concurrency/actor.h>
#include <order_matcher/central_order_book.h>
#include <fix/fix_message.h>

class OutgoingMessageProcessor : public core::Actor, public core::YieldWaitStrategy
{
    public:
        OutgoingMessageProcessor();
        void enableOfflineMode(const std::string& outputFile);
        void setMessageQueue(order_matcher::OutgoingMessageQueue* queue, int numberOfQueues);
        void* run() override;
        void rejectOrder(std::size_t sessionId, fix::FixMessage* message, const std::string& rejectionMessage);

    private:
        order_matcher::OutgoingMessageQueue* m_messageQueue = nullptr;
        int m_numberOfQueues;
        int m_execID;
        bool m_offlineMode;
        std::string m_offlineModeOutputFile;
        std::unique_ptr<order_matcher::OutgoingMessage> m_outgoingMessage;
        int generateExecID();
};

#endif