#ifndef _TCP_SERVER_THREAD_PER_CLIENT_
#define _TCP_SERVER_THREAD_PER_CLIENT_

#include "socket.h"
#include "tcp_server.h"
#include <string>
#include <vector>
#include <cstddef>

#include <core/concurrency/thread.h>
#include <core/concurrency/spinlock.hpp>

namespace core
{

class TCPServerThreadPerClient : public TCPServer
{
    public:
        TCPServerThreadPerClient() : TCPServerThreadPerClient(DEFAULT_PENDING_CONNECTION_QUEUE_SIZE, DEFAULT_ACCEPT_TIMEOUT) {}
        TCPServerThreadPerClient(int pendingConnectionsQueueSize, int acceptTimeout): m_acceptorThreadPtr{ nullptr } {}
        void* acceptThread() ;
        virtual bool start(const std::string& address, int port) override;
        virtual void stop() override;
        virtual void onClientConnected(std::size_t peerIndex) override;
        virtual void onClientDisconnected(std::size_t peerIndex) override;
    protected :
        ThreadPtr m_acceptorThreadPtr;
        mutable core::SpinLock m_peerSocketsLock;
        std::vector<ThreadPtr> m_clientThreads;
};

}//namespace

#endif