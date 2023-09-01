#include "tcp_server_reactor.h"
#include <string>
#include <cstring>
#include <utility>
using namespace std;

namespace core
{

TCPServerReactor::TCPServerReactor(int pendingConnectionsQueueSize, int acceptTimeout)
: TCPServer(pendingConnectionsQueueSize, acceptTimeout)
{
    m_reactorThreadStackSize = 0;
    m_reactorThreadCpuId = -1;
    m_reactorThreadPriority = core::ThreadPriority::NORMAL;
}

bool TCPServerReactor::start(const string& address, int port)
{
    // Start acceptor thread
    if (TCPServer::start(address, port) == false)
    {
        return false;
    }

    m_socket.setBlockingMode(false);

#ifdef __linux__
    m_ioListener.start();
    m_ioListener.addFileDescriptor(m_socket.getSocketDescriptor());
#endif

    // Start reactor thread
    TaskPtr task(new Task(&TCPServerReactor::reactorThread, this));
    m_reactorThread.reset(new Thread("TCP_REACTOR"));
    m_reactorThread->setTask(std::move(task));
    m_reactorThread->start(m_reactorThreadStackSize);
    m_reactorThread->setPriority(m_reactorThreadPriority);
    if (m_reactorThreadCpuId >= 0)
    {
        m_reactorThread->pinToCPUCore(m_reactorThreadCpuId);
    }
    return true;
}

void TCPServerReactor::stop()
{
    TCPServer::stop();

    m_reactorThread->join();

#ifdef __linux__
    m_ioListener.stop();
#endif
}

void TCPServerReactor::setPollTimeout(long microSeconds)
{
    m_ioListener.setTimeout(microSeconds);
}

void TCPServerReactor::setMaxPollEvents(size_t maxPollEvents)
{
#ifdef __linux__
    m_ioListener.setMaxPollEvents(maxPollEvents);
#endif
}

void TCPServerReactor::onClientConnected(size_t peerIndex)
{
#ifdef __linux__
    auto sd = m_peerSockets[peerIndex]->getSocketDescriptor();
    m_peerSocketIndexTable[sd] = peerIndex;
    m_ioListener.addFileDescriptor(sd);
    m_peerSockets[peerIndex]->setBlockingMode(false);
#endif
}

void TCPServerReactor::onClientDisconnected(size_t peerIndex)
{
    auto peerSocket = m_peerSockets[peerIndex].get();

#ifdef __linux__
    m_ioListener.removeFileDescriptor(peerSocket->getSocketDescriptor());
#elif _WIN32
    m_ioListener.clearFileDescriptor(peerSocket->getSocketDescriptor());
#endif

    removePeer(peerIndex);
}

size_t TCPServerReactor::acceptNewConnection()
{
    size_t peerIndex{ 0 };
    TCPConnection* peerSocket = nullptr;

    peerSocket = static_cast<TCPConnection*>(m_socket.accept(m_acceptTimeout));

    if (peerSocket)
    {
        auto peerIndex = addPeer(peerSocket);
        onClientConnected(peerIndex);
    }

    return peerIndex;
}

void* TCPServerReactor::reactorThread()
{
    while (true)
    {
        if (m_isStopping.load() == true)
        {
            break;
        }

        // Find peer count and max socket descriptor , only needed for select
#ifdef _WIN32
        m_ioListener.reset();
        m_ioListener.setFileDescriptor(m_socket.getSocketDescriptor());

        size_t connectedPeerCount{ 0 };
        int maxSocketDescriptor{ m_socket.getSocketDescriptor() };

        auto peerCount = m_peerSockets.size();

        for (size_t i{ 0 }; i < peerCount; i++)
        {
           int currentSocketDescriptor = m_peerSockets[i]->getSocketDescriptor();
           if (m_peerSocketsConnectionFlags[i] == true)
           {

                if(currentSocketDescriptor > maxSocketDescriptor)
                {
                    maxSocketDescriptor = currentSocketDescriptor;
                }

                m_ioListener.setFileDescriptor(currentSocketDescriptor);
                connectedPeerCount++;
            }
        }
#endif

        int result{ -1 };

        // Client handle events
#ifdef __linux__
        result = m_ioListener.getNumberOfReadyFileDescriptors();
#elif _WIN32
        result = m_ioListener.eventReady(maxSocketDescriptor);
#endif

        if (result > 0)
        {
#ifdef __linux__
            // We only iterate thru events with epoll instead of all socket descriptors
            auto numEvents = result;
            for (int counter{ 0 }; counter < numEvents; counter++)
            {
                size_t peerIndex = m_peerSocketIndexTable[m_ioListener.getReadyFileDescriptor(counter)];

                if (m_ioListener.isValidEvent(counter))
                {
                    auto currentSocketDescriptor = m_ioListener.getReadyFileDescriptor(counter);
                    if (currentSocketDescriptor == m_socket.getSocketDescriptor())
                    {
                        acceptNewConnection();
                    }
                    else
                    {
                        handleClient(peerIndex);
                    }
                }
                else
                {
                    onClientDisconnected(peerIndex);
                }
            }
#elif _WIN32
            // Accept new connections
            if (m_ioListener.isFileDescriptorReady(m_socket.getSocketDescriptor()))
            {
                acceptNewConnection();
            }

            // We have to iterate thru all sockets with select
            for (int counter{ 0 }; counter < peerCount; counter++)
            {
                if (m_ioListener.isFileDescriptorReady(m_peerSockets[counter]->getSocketDescriptor()))
                {
                    handleClient(counter);
                }
            }
#endif
        }
        else if (result == 0)
        {
            // Timeout
        }
        else
        {
            auto errorCode = Socket::getCurrentThreadLastSocketError();
            onUnhandledSocketError(errorCode, result);
        }
    }
    return nullptr;
}

}//namespace