#ifndef _TCP_SERVER_
#define _TCP_SERVER_

#include "tcp_connection.h"
#include <string>
#include <memory>
#include <atomic>
#include <vector>
#include <cstddef>

namespace core
{

#define DEFAULT_PENDING_CONNECTION_QUEUE_SIZE 32
#define DEFAULT_ACCEPT_TIMEOUT 5

class TCPServer
{
    public:
        TCPServer() : TCPServer(DEFAULT_PENDING_CONNECTION_QUEUE_SIZE, DEFAULT_ACCEPT_TIMEOUT) {}
        TCPServer(int pendingConnectionsQueueSize, int acceptTimeout);
        virtual ~TCPServer();

        virtual bool start(const std::string& address, int port);
        virtual void stop();
        TCPConnection* getAcceptorSocket() { return &m_socket; }
        TCPConnection* getPeerSocket(std::size_t peerIndex) { return m_peerSockets[peerIndex].get(); }

        // Abstract methods
        virtual void onClientConnected(std::size_t peerIndex) = 0;
        virtual void onClientDisconnected(std::size_t peerIndex) = 0;
        virtual void onUnhandledSocketError(int errorCode, int eventResult) = 0;
        virtual void* handleClient(std::size_t peerIndex) = 0;

    protected:
        int m_acceptTimeout;
        std::atomic<bool> m_isStopping;
        std::vector<std::unique_ptr<TCPConnection>> m_peerSockets;
        std::vector<bool> m_peerSocketsConnectionFlags;
        TCPConnection m_socket;
        std::size_t addPeer(TCPConnection* peer);
        void removePeer(std::size_t peerIndex);
};

}//namespace

#endif