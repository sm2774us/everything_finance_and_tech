#ifndef _FIX_SERVER_REACTOR_
#define _FIX_SERVER_REACTOR_

#include <cstddef>
#include <vector>
#include <memory>
#include <core/network/tcp_server_reactor.h>
#include <core/concurrency/spinlock.hpp>
#include "fix_session.h"
#include "fix_message.h"
#include "fix_server_configuration.h"

namespace fix
{

class FixServerReactor : public core::TCPServerReactor
{
    public :
        FixServerReactor();
        void initialise(const FixServerConfiguration& configuration);
        virtual ~FixServerReactor(){}

        void start();
        virtual void stop() override;

        virtual void onClientConnected(std::size_t peerIndex) override;
        virtual void onClientDisconnected(std::size_t peerIndex) override;
        virtual void* handleClient(std::size_t peerIndex) override;

        virtual void onFixLogon(FixSession*) = 0;
        virtual void onFixLogoff(FixSession*) = 0;
        virtual void onTraderLogon(FixSession*, const std::string&) = 0;
        virtual void onFixMessage(FixMessage*, std::size_t) = 0;
        virtual void onFixError(const std::string&, FixSession*) = 0;

        static FixSession* getSession(std::size_t);

    protected :
        std::size_t m_receiveCacheSize;
        bool m_sequenceNumberValidation;
        int m_port;
        std::string m_address;
        core::Subseconds m_timePrecision;

        void processMessage(FixMessage*, FixSession*, std::size_t peerIndex);
        void checkErrors(int recvSize, FixSession*, std::size_t);
    private:
        static std::vector<std::unique_ptr<FixSession>> Sessions;
        static std::vector<bool> SessionFlags;
        static void addSession(FixSession*);
        static void removeSession(std::size_t sessionIndex);
};

}

#endif