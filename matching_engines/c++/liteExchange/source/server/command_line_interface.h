#ifndef _CLI_H_
#define _CLI_H_

#include <string>
#include <iostream>

#include <core/string_utility.h>
#include <core/concurrency/actor.h>

#include <order_matcher/central_order_book.h>
#include <order_matcher/central_order_book_visitor.h>


class CommandLineInterface : public core::Actor
{
    public :

        CommandLineInterface() : core::Actor("AdminInterface"), m_parentCentralOrderBook{ nullptr }
        {

        }

        void setParentCentralOrderbook(order_matcher::CentralOrderBook* parent)
        {
            m_parentCentralOrderBook = parent;
        }

        virtual void* run() override
        {
            displayUsage();

            while (true)
            {
                std::string value;
                std::cin >> value;

                value = core::toLower(value);

                if (value == "display")
                {
                    outputMessage("All orders in the central order book :");
                    outputMessage("");
                    outputMessage(getAllOrderBookAsString());
                }
                else if (value == "quit")
                {
                    outputMessage("Quit message received");
                    sendFinishSignal();
                    break;
                }
                else
                {
                    outputMessage("Invalid user command");
                    displayUsage();
                }

                std::cout << std::endl;
            }

            return nullptr;
        }

    private :

        order_matcher::CentralOrderBook* m_parentCentralOrderBook;

        void displayUsage()
        {
            outputMessage("");
            outputMessage("Available commands :");
            outputMessage("");
            outputMessage("\tdisplay : Shows all order books in the central order book");
            outputMessage("\tquit : Shutdowns the server");
            outputMessage("");
        }

        std::string getAllOrderBookAsString()
        {
            order_matcher::CentralOrderBookVisitor visitor;
            m_parentCentralOrderBook->accept(visitor);
            return visitor.toString();
        }

        void outputMessage(const std::string& message)
        {
            std::cout << message << std::endl;
        }
};

#endif