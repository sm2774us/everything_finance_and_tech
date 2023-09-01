#include "tcp_server_thread_per_client.h"
#include <mutex>
#include <utility>
using namespace std;

namespace core
{

bool TCPServerThreadPerClient::start(const string& address, int port)
{
    if (TCPServer::start(address, port) == false)
    {
        return false;
    }

    TaskPtr task(new Task(&TCPServerThreadPerClient::acceptThread, this));
    m_acceptorThreadPtr.reset(new Thread("TCP_ACCEPTOR"));
    m_acceptorThreadPtr->setTask(std::move(task));
    m_acceptorThreadPtr->start();

    return true;
}

void TCPServerThreadPerClient::stop()
{
    TCPServer::stop();

    m_acceptorThreadPtr->join();

    for (auto& clientThread : m_clientThreads)
    {
        clientThread->join();
    }
}

void TCPServerThreadPerClient::onClientConnected(size_t peerIndex)
{
    TaskPtr task(new Task(&TCPServerThreadPerClient::handleClient, this, peerIndex));

    ThreadPtr clientHandlerThread(new Thread("TCP_WORKER"));
    m_clientThreads.push_back(std::move(clientHandlerThread));

    clientHandlerThread->setTask(std::move(task));
    clientHandlerThread->start();
}

void TCPServerThreadPerClient::onClientDisconnected(size_t peerIndex)
{
    std::lock_guard<core::SpinLock> guard(m_peerSocketsLock);
    removePeer(peerIndex);
}

void* TCPServerThreadPerClient::acceptThread()
{
    while (true)
    {

        if (m_isStopping.load() == true)
        {
            break;
        }

        auto peerSocket = static_cast<TCPConnection*>(m_socket.accept(m_acceptTimeout));

        if (peerSocket)
        {
            std::lock_guard<core::SpinLock> guard(m_peerSocketsLock);
            auto peerIndex = addPeer(peerSocket);
            onClientConnected(peerIndex);
        }
    }
    return nullptr;
}

}//namespace