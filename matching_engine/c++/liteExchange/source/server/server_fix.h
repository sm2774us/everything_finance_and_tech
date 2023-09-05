#ifndef _SERVER_FIX_H_
#define _SERVER_FIX_H_

#include <cstddef>
#include <string>
#include <vector>
#include <memory>

#include <server/server_base.h>
#include <server/server_configuration.h>
#include <server/server_outgoing_message_processor.h>

#include <fix/fix_session.h>
#include <fix/fix_message.h>
#include <fix/fix_server_reactor.h>

class ServerFix : public ServerBase, public fix::FixServerReactor
{
    public:
        explicit ServerFix(const ServerConfiguration& serverConfiguration);
        virtual ~ServerFix();
        order_matcher::CentralOrderBook& getCentralOrderBook() { return m_centralOrderBook; }
        virtual void run() override;

        virtual void onUnhandledSocketError(int errorCode, int eventResult) override;
        virtual void onFixError(const std::string&, fix::FixSession*) override;

        virtual void onFixLogon(fix::FixSession*) override;
        virtual void onFixLogoff(fix::FixSession*) override;
        virtual void onTraderLogon(fix::FixSession*, const std::string&) override;
        virtual void onFixMessage(fix::FixMessage*, std::size_t) override;

        void onNewOrder(fix::FixMessage*, std::size_t);
        void onCancelOrder(fix::FixMessage*, std::size_t);
};

#endif