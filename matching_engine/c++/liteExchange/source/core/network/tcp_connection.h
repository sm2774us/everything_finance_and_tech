#ifndef _TCP_CONNECTION_
#define _TCP_CONNECTION_

#include "socket.h"
#include <string>
#include <cstddef>

namespace core
{

class TCPConnection : public Socket
{
    public:
        bool create();
        int receive(char* buffer, std::size_t len, int timeout = 0);
        int send(const std::string& buffer, int timeout = 0);
        int send(const char* buffer, std::size_t len, int timeout = 0);

        void* getContextData() const { return m_contextData; }
        void setContextData(void* contextData) { m_contextData = contextData; }

    private:
        void* m_contextData = nullptr;
};

}//namespace

#endif