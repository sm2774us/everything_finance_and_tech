#include "server_offline.h"

#include <core/file_utility.h>
#include <core/string_utility.h>
#include <core/logger/logger.h>
#include <core/concurrency/thread.h>

#include <fix/fix_constants.h>
#include <fix/fix_message.h>

#include <order_matcher/security_manager.h>

#include <server/server_constants.h>
#include <server/fix_message_converter.h>

#include <exception>
#include <fstream>
using namespace std;
using namespace order_matcher;

ServerOffline::ServerOffline(const ServerConfiguration& serverConfiguration) :ServerBase(serverConfiguration)
{
    // INPUT FILE
    m_file = serverConfiguration.getOfflineOrderEntryFile();
    // OUTPUT FILE
    m_outgoingMessageProcessor.enableOfflineMode(serverConfiguration.getOfflineOrderEntryOutputFile());
}

void ServerOffline::loadOrders()
{
    if (core::doesFileExist(m_file) == false)
    {
        return;
    }

    vector<fix::FixMessage> fixMessages;
    fix::FixMessage::loadFromFile(m_file, fixMessages);

    for(const auto& fixMessage : fixMessages)
    {
        IncomingMessageType type;


        Order order;
        auto messageType = fixMessage.getMessageType();

        // ORDER MESSAGE TYPE
        if (messageType == fix::FixConstants::MessageType::NEW_ORDER)
        {
            type = IncomingMessageType::NEW_ORDER;
            FixMessageConverter::convertNewOrder(fixMessage, order);
        }
        else if (messageType == fix::FixConstants::MessageType::CANCEL)
        {
            type = IncomingMessageType::CANCEL_ORDER;
            FixMessageConverter::convertCancelOrder(fixMessage, order);
        }
        else
        {
            LOG_WARNING("FixMessageConverter", "Only new orders and cancel orders are supported")
            continue;
        }

        IncomingMessage message(order, type);
        m_orders.emplace_back(message);
    }

}

void ServerOffline::run()
{
    m_commandLineInterface.start();
    loadOrders();

    LOG_INFO("Offline Server", "Loaded all orders into memory")

    for (auto& order : m_orders)
    {
        m_centralOrderBook.addOrder(order.getOrder());
    }

    LOG_INFO("Offline Server", "Submitted all orders to the central order book")

    while (true)
    {
        // Exit if interface returns false , it means we are exiting
        if (m_commandLineInterface.isFinishing())
        {
            break;
        }

        for (auto& thread : core::Thread::THREADS)
        {
            auto threadException = thread->getException();
            if (threadException)
            {
                LOG_ERROR("Offline Server", "Unhandled exception in thread : " + thread->getName())
                    std::rethrow_exception(threadException);
            }
        }
    }
}