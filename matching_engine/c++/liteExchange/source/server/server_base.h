#ifndef _SERVER_BASE_H_
#define _SERVER_BASE_H_

#include <core/noncopyable.h>
#include <core/concurrency/thread_priority.h>

#include <server/command_line_interface.h>
#include <server/server_configuration.h>
#include <server/server_outgoing_message_processor.h>

#include <fix/fix_constants.h>
#include <fix/fix_message.h>

#include <order_matcher/order.h>
#include <order_matcher/central_order_book.h>

class ServerBase : public core::NonCopyable
{
    public :
        explicit ServerBase(const ServerConfiguration& serverConfiguration)
        {
            // Central order book initialisation
            auto symbols = serverConfiguration.getSymbols();
            auto symbolCount = symbols.size();
            m_centralOrderBook.setSymbols(symbols);

            if (serverConfiguration.getMatchingMultithreadingMode() == true)
            {
                core::ThreadPoolArguments args = serverConfiguration.getThreadPoolArguments();
                args.m_threadNames = symbols;
                m_centralOrderBook.initialiseMultithreadedMatching(args);
            }

            m_centralOrderBook.initialiseOutgoingMessageQueues(symbolCount, serverConfiguration.getOutgoingMessageQueueSizePerThread());

            // Outgoing message processor initialisation
            m_outgoingMessageProcessor.setMessageQueue(m_centralOrderBook.getOutgoingMessageQueue(), symbolCount);
            m_outgoingMessageProcessor.start(serverConfiguration.getOutgoingMessageThreadStackSize());

            auto outgoingMessageProcessorCpuId = serverConfiguration.getOutgoingMessageThreadCPUId();
            if (outgoingMessageProcessorCpuId >= 0)
            {
                m_outgoingMessageProcessor.pinToCPUCore(outgoingMessageProcessorCpuId);
            }

            m_outgoingMessageProcessor.setPriority(serverConfiguration.getOutgoingMessageThreadPriority());
        }

        virtual void run() = 0;

    protected:
        order_matcher::CentralOrderBook m_centralOrderBook;
        OutgoingMessageProcessor m_outgoingMessageProcessor;
        CommandLineInterface m_commandLineInterface;
};

#endif