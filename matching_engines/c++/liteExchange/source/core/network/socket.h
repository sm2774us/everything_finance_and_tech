#ifndef _SOCKET_
#define _SOCKET_

#include "address.h"
#include <string>
#include <cstddef>

namespace core
{

enum class SOCKET_TYPE
{
    TCP,
    UDP
};

enum class SOCKET_OPTION
{
    GET_ERROR_AND_CLEAR,
    REUSE_ADDRESS,
    REUSE_PORT,
    EXCLUSIVE_ADDRESS,
    RECEIVE_BUFFER_SIZE,
    RECEIVE_BUFFER_TIMEOUT,
    SEND_BUFFER_SIZE,
    SEND_BUFFER_TIMEOUT,
    TCP_ENABLE_CORK,
    TCP_ENABLE_QUICKACK,              // Applies only to Linux , even Nagle is turned off , delayed can cause time loss due in case of lost packages
    TCP_DISABLE_NAGLE,          // Send packets as soon as possible , no need to wait for ACKs or to reach a certain amount of buffer size
    POLLING_INTERVAL,           // SO_BUSY_POLL , specifies time to wait for select to query kernel to know if new data received
    SOCKET_PRIORITY
};

enum class SOCKET_STATE
{
    DISCONNECTED,
    BOUND,
    CONNECTED,
    LISTENING,
    ACCEPTED
};

class Socket
{
    public :
        Socket():m_socketDescriptor{0}, m_state{ SOCKET_STATE::DISCONNECTED }, m_pendingConnectionsQueueSize{0}
        {}

        ~Socket();

        bool create(SOCKET_TYPE type);
        void close();
        bool listen();
        bool bind(const std::string& address, int port);

        bool connect(const std::string& address, int port);
        bool connect(const std::string& address, int port, int timeout);

        Socket* accept(int timeout);
        bool select(bool read, bool write, long timeout);

        void setPendingConnectionsQueueSize(int value) { m_pendingConnectionsQueueSize = value; }
        void setBlockingMode(bool blockingMode);

        void setSocketOption(SOCKET_OPTION option, int value);
        int getSocketOption(SOCKET_OPTION option);

        int getLastSocketError();
        static int getCurrentThreadLastSocketError();
        static std::string getSocketErrorAsString(int errorCode);
        bool isConnectionLost(int errorCode, std::size_t receiveResult);

        SOCKET_STATE getState() const { return m_state; }
        int getPort() const { return m_address.getPort(); }
        std::string getAddress() const{ return m_address.getAddress(); }
        int getSocketDescriptor() const { return m_socketDescriptor; }

    protected:
        int m_socketDescriptor;

    private:
        SOCKET_STATE m_state;
        int m_pendingConnectionsQueueSize;
        SOCKET_TYPE m_socketType;
        Address m_address;

        // Move ctor deletion
        Socket(Socket&& other) = delete;
        // Move assignment operator deletion
        Socket& operator=(Socket&& other) = delete;

        void initialise(const std::string& address, int port);
        void initialise(int socketDescriptor, struct sockaddr_in* socketAddress);
        int getSocketOptionValue(SOCKET_OPTION);
};

}//namespace

#endif