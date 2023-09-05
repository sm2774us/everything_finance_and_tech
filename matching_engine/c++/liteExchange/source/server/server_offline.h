#ifndef _SERVER_OFFLINE_H_
#define _SERVER_OFFLINE_H_

#include <string>
#include <vector>

#include <server/server_base.h>
#include <order_matcher/incoming_message.h>
#include <server/server_configuration.h>

class ServerOffline : public ServerBase
{
    public :
        explicit ServerOffline(const ServerConfiguration& serverConfiguration);
        virtual void run() override;

    private :
        void loadOrders();
        std::string m_file;
        std::vector<order_matcher::IncomingMessage> m_orders;
};

#endif