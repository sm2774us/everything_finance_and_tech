#include "thread.h"
#include "thread_priority.h"
#include <cstdlib>
#include <cassert>
#include <memory>
#include <type_traits>

#ifdef __linux__
#include <signal.h>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include <sched.h>
#endif

using namespace std;

namespace core
{

vector<Thread*> Thread::THREADS;

Thread::Thread(const string& name) : m_task{ nullptr }, m_name(name), m_started{ false },
m_joined{ false }, m_threadID{ 0 }, m_priority{ ThreadPriority::NORMAL }
{
#ifdef _WIN32
    m_threadHandle = nullptr;
#endif
    Thread::THREADS.emplace_back(this);
}

Thread::~Thread()
{
    #if DEBUG
    if ( isAlive() == true )
    {
        // Join might be active on another thread so let`s wait
        Thread::sleep(THREAD_WAIT_TIMEOUT);
        if ( isAlive() == true )
        {
            // If the thread function is still running then we have a problem in the logic
            // as this thread meant to be used via Actor or ThreadPool classes
            // or it is taking too long
            string threadName = m_name.length() > 0 ? m_name : "unknown";
            string message = "Thread is still running , thread name : " + threadName;

            assert(1==0 && message.c_str());
        }
    }
    #endif
    // NOTE :For Windows , no need to call CloseHandle for _beginthread
    //As soon the thread function returns _endthread will be called by Windows CRT
    //And it will call CloseHandle
}

void Thread::setThreadName()
{
    if ( m_name.length() > 0)
    {
#ifdef __linux__
        assert( m_name.length() < MAX_THREAD_NAME_LENGTH ); // Linux thread names can`t have a longer name

        pthread_setname_np(m_threadID, m_name.c_str());
#elif _WIN32
        // As documented on MSDN
        // https://msdn.microsoft.com/en-us/library/xcb2z8hs(v=vs.120).aspx
        const DWORD MS_VC_EXCEPTION = 0x406D1388;

        #pragma pack(push,8)
        typedef struct tagTHREADNAME_INFO
        {
            DWORD dwType; // Must be 0x1000.
            LPCSTR szName; // Pointer to name (in user addr space).
            DWORD dwThreadID; // Thread ID (-1=caller thread).
            DWORD dwFlags; // Reserved for future use, must be zero.
        } THREADNAME_INFO;
        #pragma pack(pop)

        THREADNAME_INFO info;
        info.dwType = 0x1000;
        info.szName = m_name.c_str();
        info.dwThreadID = m_threadID;
        info.dwFlags = 0;

        __try
        {
            RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
        }
#endif
    }
}

void Thread::start(size_t stackSize)
{
    bool success = true;
 #ifdef __linux__
    pthread_attr_init(&m_threadAttr);

    if(stackSize != 0)
    {
        pthread_attr_setstack(&m_threadAttr, NULL, stackSize);
    }

    if( pthread_create(&m_threadID, NULL, internalThreadFunction, m_task.get()) != 0 )
    {
        success = false;
    }
#elif _WIN32
    // Note 1
    // Why not using CreateThread From MSDN
    // A thread in an executable that calls the C run-time library (CRT) should use the _beginthreadex and _endthreadex functions
    // for thread management rather than CreateThread and ExitThread; this requires the use of the multithreaded version of the CRT.
    // If a thread created using CreateThread calls the CRT, the CRT may terminate the process in low-memory conditions

    // Note 2
    // Why preferring _beginthread over _beginthreadex
    // It is mainly I can specify a worker function with void* retval with pthreads
    // Therefore I can use same internal function in Windows and Linux
    // The disadvantage is that , you can`t Wait on _beginthread-created threads as CRT closes the handle

    // Note 3
    // We need C style casting here as static_cast fails to do this function pointer conversion
    // In C standards, this typecast might lead to undefined behaviour :
    // http://stackoverflow.com/questions/559581/casting-a-function-pointer-to-another-type
    // Works fine for Windows
    m_threadHandle = (HANDLE) _beginthread((void(__cdecl *)(void *))(internalThreadFunction), stackSize, m_task.get());

    m_threadID = GetThreadId(m_threadHandle);

    if ( m_threadID == 0)
    {
        success = false;
    }
#endif

    if (success == false)
    {
        throw std::runtime_error("Thread creation failed");
    }

    m_started = true;
    setThreadName();
}

bool Thread::setPriority(ThreadPriority priority)
{
    assert(m_started == true);
    bool success{ false };
    int index = static_cast<std::underlying_type<ThreadPriority>::type >(priority);
    auto nativePriorityValue = NATIVE_THREAD_PRIORITIES[index].value;
#ifdef __linux__
    struct sched_param param;
    param.__sched_priority = nativePriorityValue;

    int policy = sched_getscheduler(getpid());

    pthread_setschedparam(pthread_self(), policy, &param);
#elif _WIN32
    if (SetThreadPriority(m_threadHandle, nativePriorityValue) != 0)
    {
        success = true;
    }
#endif
    m_priority = priority;
    return success;
}

int Thread::getRealPriority()
{
    int ret{ -1 };
#ifdef __linux__
    struct sched_param param;
    sched_getparam(getpid(), &param);
    ret = param.__sched_priority;
#elif _WIN32
    ret = GetThreadPriority(m_threadHandle);
#endif
    return ret;
}

int Thread::pinToCPUCore(int coreID)
{
    assert( m_started == true );
    int ret{ -1 };
#ifdef __linux__
    ret = Thread::pinToCPUCoreInternal(coreID, m_threadID);
#elif _WIN32
    ret = Thread::pinToCPUCoreInternal(coreID, m_threadHandle);
#endif
    return ret;
}

#ifdef __linux__
int Thread::pinToCPUCoreInternal(int coreID, unsigned long threadID)
#elif _WIN32
int Thread::pinToCPUCoreInternal(int coreID, HANDLE handle)
#endif
{
    assert(coreID >= 0);
#ifdef __linux__
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(coreID, &cpuset);
    return pthread_setaffinity_np(threadID, sizeof(cpu_set_t), &cpuset);
#elif _WIN32
    unsigned long mask = 1 << (coreID);

    if (!SetThreadAffinityMask(handle, mask))
    {
        return -1;
    }

    return 0;
#endif
}

bool Thread::isAlive() const
{
    if (m_started == false)
    {
        return false;
    }

    bool ret = false;
#ifdef __linux__
    // No signal is sent, but error checking is still performed so you can use that to check existence of tid :
    // On success, pthread_kill() returns 0

    // Thing I have experimented is that this method in debug mode ( gcc -g ) will cause a segmentation fault
    // However works fine in release mode
#if !DEBUG
    if( pthread_kill(m_threadID, 0) == 0 )
    {
        ret = true;
    }
#endif
#elif _WIN32
    DWORD exitCode = 0;
    GetExitCodeThread(m_threadHandle, &exitCode);

    if (exitCode == STILL_ACTIVE) // the thread handle is not signaled - the thread is still alive
    {
        ret = true;
    }
#endif
    return ret;
}

bool Thread::isJoiningOk() const
{
    if (m_started == false || m_joined == true) // joining a thread for 2nd time in Linux is causing sigsegv
    {
        return false;
    }
    return true;
}

void Thread::join()
{
    if ( isJoiningOk() == false )
    {
        return;
    }

#ifdef __linux__
    pthread_join(m_threadID, NULL);
#elif _WIN32
    //We use _beginthread therefore we can`t really wait for the handle
    //CRT will close the thread handle so we have to block till it is closed
    while (true)
    {
        auto result = WaitForSingleObject(m_threadHandle, 1000);
        if (result == WAIT_OBJECT_0)
        {
            break;
        }
        else if (result == WAIT_FAILED)
        {
            if (GetLastError() == ERROR_INVALID_HANDLE)
            {
                break;
            }
        }
    }

#endif
    m_joined = true;
}

void Thread::detach()
{
#ifdef __linux__
    pthread_t t = m_threadID;
    pthread_detach(t);
#elif _WIN32
    CloseHandle(m_threadHandle);
#endif
}

void* Thread::internalThreadFunction(void* argument)
{
    assert(argument != nullptr);
    Task* task = static_cast<Task *>(argument);
    task->execute();
    return nullptr;
}

int Thread::pinCallingThreadToCPUCore(int coreID)
{
    int ret{ -1 };
#ifdef __linux__
    ret = Thread::pinToCPUCoreInternal(coreID, pthread_self());
#elif _WIN32
    ret = Thread::pinToCPUCoreInternal(coreID, GetCurrentThread());
#endif
    return ret;
}

int Thread::getCurrentCoreID()
{
   int current_core_id{ -1 };
#ifdef __linux__
   current_core_id = ::sched_getcpu();
#elif _WIN32
   current_core_id = ::GetCurrentProcessorNumber();
#endif
    return current_core_id;
}

unsigned int Thread::getNumberOfCores()
{
    // C++11 way std::thread::hardware_concurrency()
    unsigned int numCores(0);
#ifdef __linux__
    numCores = sysconf( _SC_NPROCESSORS_ONLN );
#elif _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    numCores = sysinfo.dwNumberOfProcessors;
#endif
    return numCores;
}

unsigned long Thread::getCurrentThreadID()
{
    // C++11 way std::this_thread::get_id()
    auto threadID(0);
#ifdef __linux__
    threadID = pthread_self();
#elif _WIN32
    threadID = ::GetCurrentThreadId();
#endif
    return threadID;
}

bool Thread::isHyperThreading()
{
    bool ret = true;
#ifdef __linux__
    // As POSIXs don`t give us a way,
    // we can do this via CPUID instruction
    // https://en.wikipedia.org/wiki/CPUID

    // Here is CPUID dumps for AMD processors :
    // http://users.atw.hu/instlatx64/
    uint32_t registers[4];
    __asm__ __volatile__ ("cpuid " :
                      "=a" (registers[0]),
                      "=b" (registers[1]),
                      "=c" (registers[2]),
                      "=d" (registers[3])
                      : "a" (1), "c" (0));

    unsigned CPUFeatureSet = registers[3];
    ret = CPUFeatureSet & (1 << 28);
#elif _WIN32
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = nullptr;
    DWORD bufferLen = 0;
    DWORD logicalProcessorCount = 0;
    DWORD processorCoreCount = 0;

    // Get the size first
    GetLogicalProcessorInformation(nullptr, &bufferLen);
    // Allocate buffer
    auto buffer_deleter = [](SYSTEM_LOGICAL_PROCESSOR_INFORMATION* memory_to_delete)-> void { free(memory_to_delete); };
    std::unique_ptr <SYSTEM_LOGICAL_PROCESSOR_INFORMATION, decltype(buffer_deleter)> buffer((PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(bufferLen), buffer_deleter);

    GetLogicalProcessorInformation(buffer.get(), &bufferLen);

    DWORD byteOffset = 0;
    ptr = buffer.get();

    // Local lambda function
    auto countSetBits = [&](ULONG_PTR bitMask) -> DWORD
    {
        DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
        DWORD bitSetCount = 0;
        ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
        DWORD i;

        for (i = 0; i <= LSHIFT; ++i)
        {
            bitSetCount += ((bitMask & bitTest) ? 1 : 0);
            bitTest /= 2;
        }

        return bitSetCount;
    };

    while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= bufferLen)
    {
        switch (ptr->Relationship)
        {
            case RelationProcessorCore:
                processorCoreCount++;
                // A hyperthreaded core supplies more than one logical processor.
                logicalProcessorCount += countSetBits(ptr->ProcessorMask);
                break;
            default:
                break;
        }
        byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptr++;
    }

    if (logicalProcessorCount == processorCoreCount)
    {
        ret = false;
    }
#endif
    return ret;
}

}//namespace