#include "io_listener_select.h"

namespace core
{

IOListenerSelect::IOListenerSelect()
{
    reset();
    setTimeout(50000);
}

IOListenerSelect::~IOListenerSelect()
{
}

void IOListenerSelect::setTimeout(long microSeconds)
{
    m_timeout.tv_sec = (microSeconds / 1000000);
    m_timeout.tv_usec = 0;
}

void IOListenerSelect::setFileDescriptor(int fd)
{
    FD_SET(fd, &m_clientsReadSet);
}

void IOListenerSelect::reset()
{
    FD_ZERO(&m_clientsReadSet);
}

void IOListenerSelect::clearFileDescriptor(int fd)
{
    FD_CLR(fd, &m_clientsReadSet);
}

int IOListenerSelect::eventReady(int maxSocketDescriptor)
{
    int ret{-1};

    ret = ::select(maxSocketDescriptor+1, &m_clientsReadSet, nullptr, nullptr, &m_timeout);

    return ret;
}

bool IOListenerSelect::isFileDescriptorReady(int fd)
{
    bool ret{false};

    ret = (FD_ISSET(fd, &m_clientsReadSet))?true : false;

    return ret;
}

}//namespace