#ifndef _IO_LISTENER_SELECT_
#define _IO_LISTENER_SELECT_

#ifdef __linux__
#include <sys/select.h>
#include <sys/time.h>
#elif _WIN32
#include <Ws2tcpip.h>
#endif

namespace core
{

class IOListenerSelect
{
    public:
        IOListenerSelect();
        ~IOListenerSelect();
        void setTimeout(long microSeconds);
        void setFileDescriptor(int fd);
        void reset();
        void clearFileDescriptor(int fd);
        int eventReady(int maxSocketDescriptor);
        bool isFileDescriptorReady(int fd);
    private:
        struct timeval m_timeout;
        fd_set m_clientsReadSet;
};

}//namespace

#endif