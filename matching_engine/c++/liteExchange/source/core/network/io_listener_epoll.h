#ifndef _IO_LISTENER_EPOLL_
#define _IO_LISTENER_EPOLL_

#include <cstddef>
#include <sys/time.h>
#include <sys/epoll.h>

namespace core
{

enum class EPOLL_MODE
{
    LEVEL_TRIGGERED,
    EDGE_TRIGGERED
};

class IOListenerEpoll
{
    public:
        IOListenerEpoll();
        ~IOListenerEpoll();
        void start();
        void stop();
        void setTimeout(long microSeconds);
        void addFileDescriptor(int fd);
        void removeFileDescriptor(int fd);
        int getNumberOfReadyFileDescriptors();
        bool isValidEvent(int index);
        int getReadyFileDescriptor(int index);
        void setEpollMode(EPOLL_MODE mode) { m_epollMode = mode; }
        void setMaxPollEvents(std::size_t maxPollEvents) { m_maxPollEvents = maxPollEvents; }
        EPOLL_MODE getEpollMode () const { return m_epollMode; }
    private:
        EPOLL_MODE m_epollMode;
        int m_epollDescriptor;
        struct epoll_event* m_epollEvents;
        int m_epollTimeout;
        std::size_t m_maxPollEvents;
};

}//namespace

#endif