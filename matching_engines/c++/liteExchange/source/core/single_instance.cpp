#include "single_instance.h"

#ifdef __linux__
#include <unistd.h>
#include <fcntl.h>
#include <core/string_utility.h>
#endif

using namespace std;

namespace core
{

SingleInstance::SingleInstance()
{
#ifdef __linux__
    m_fd = 0;
#elif _WIN32
    m_mutex = nullptr;
#endif
}

SingleInstance::~SingleInstance()
{
#ifdef __linux__
    if (m_fd > 0)
    {
        unlink(m_filePath.c_str());
    }
#elif _WIN32
    if (m_mutex)
    {
        CloseHandle(m_mutex);
        m_mutex = NULL;
    }
#endif
}

bool SingleInstance::operator()()
{
#ifdef __linux__
    struct flock fl;

    m_filePath = core::format("%s/%s", getenv("HOME"), SINGLE_INSTANCE_LOCK);

    m_fd = open(m_filePath.c_str(), O_RDWR | O_CREAT, 0600);

    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    if (fcntl(m_fd, F_SETLK, &fl) < 0)
    {
        return false;
    }
    return true;
#elif _WIN32
    m_mutex = CreateMutex(NULL, FALSE, SINGLE_INSTANCE_LOCK);
    m_lastError = GetLastError();
    return (ERROR_ALREADY_EXISTS == m_lastError)?false:true;
#endif
}

}//namespace