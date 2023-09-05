#ifndef _ADDRESS_
#define _ADDRESS_

#ifdef __linux__
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#elif _WIN32
#include <Ws2tcpip.h>
#endif

#include <cstring>
#include <string>

namespace core
{

class Address
{
    public:
        Address() : m_port{0}
        {}

        void initialise(const std::string& address, int port)
        {
            m_port = port;
            m_address = address;

            auto addr = &m_socketAddressStruct;

            memset(addr, 0, sizeof(sockaddr_in));
            addr->sin_family = PF_INET;
            addr->sin_port = htons(port);

            if (m_address.size() > 0)
            {
                if (getAddressInfo(m_address.c_str(), &(addr->sin_addr)) != 0)
                {
                    inet_pton(PF_INET, m_address.c_str(), &(addr->sin_addr));
                }
            }
            else
            {
                addr->sin_addr.s_addr = INADDR_ANY;
            }
        }

        void initialise(struct sockaddr_in* socketAddressStruct)
        {
            char ip[50];
#ifdef __linux__
            inet_ntop(PF_INET, (struct in_addr*)&(socketAddressStruct->sin_addr.s_addr), ip, sizeof(ip) - 1);
#elif _WIN32
            InetNtopA(PF_INET, (struct in_addr*)&(socketAddressStruct->sin_addr.s_addr), ip, sizeof(ip) - 1);
#endif
            m_address = ip;
            m_port = ntohs(socketAddressStruct->sin_port);
        }

        const std::string& getAddress() const { return m_address; }
        int getPort() const { return m_port; }
        struct sockaddr_in* getSocketAddressStruct() { return &m_socketAddressStruct; }

    private:
        int m_port;
        std::string m_address;
        struct sockaddr_in m_socketAddressStruct;

        static int getAddressInfo(const char* hostname, struct in_addr* socketAddress)
        {
            struct addrinfo *res{ nullptr };

            int result = getaddrinfo(hostname, nullptr, nullptr, &res);

            if (result == 0)
            {
                memcpy(socketAddress, &((struct sockaddr_in *) res->ai_addr)->sin_addr, sizeof(struct in_addr));
                freeaddrinfo(res);
            }

            return result;
        }
};

}//namespace

#endif