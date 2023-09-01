#include "socket_library.h"
#include "tcp_server.h"
#include "tcp_connection.h"
using namespace std;

namespace core
{

TCPServer::TCPServer(int pendingConnectionsQueueSize, int acceptTimeout)
    : m_acceptTimeout{ acceptTimeout }
{
    SocketLibrary::initialise();
    m_isStopping.store(false);
    m_socket.create();
    m_socket.setPendingConnectionsQueueSize(pendingConnectionsQueueSize);
}

TCPServer::~TCPServer()
{
    stop();
    SocketLibrary::uninitialise();
}

bool TCPServer::start(const string& address, int port)
{
    m_socket.setSocketOption(SOCKET_OPTION::REUSE_ADDRESS, 1);

    if (!m_socket.bind(address, port))
    {
        return false;
    }

    if (!m_socket.listen())
    {
        return false;
    }

    return true;
}

void TCPServer::stop()
{
    m_isStopping.store(true);
}

size_t TCPServer::addPeer(TCPConnection* peer)
{
    auto currentSize = m_peerSockets.size();
    int nonUserPeerIndex{ -1 };
    int ret{ -1 };

    for (size_t i{ 0 }; i < currentSize; i++)
    {
        if (m_peerSocketsConnectionFlags[i] == false)
        {
            nonUserPeerIndex = i;
            break;
        }
    }

    if (nonUserPeerIndex == -1)
    {
        // No empty slot , create new
        m_peerSocketsConnectionFlags.push_back(true);
        m_peerSockets.emplace_back(peer);
        ret = m_peerSockets.size() - 1;
    }
    else
    {
        // Use an existing peer slot
        m_peerSockets[nonUserPeerIndex].reset(peer);
        m_peerSocketsConnectionFlags[nonUserPeerIndex] = true;
        ret = nonUserPeerIndex;
    }
    return ret;
}

void TCPServer::removePeer(std::size_t peerIndex)
{
    auto peerSocket = getPeerSocket(peerIndex);
    m_peerSocketsConnectionFlags[peerIndex] = false;
    peerSocket->close();
}

}//namespace