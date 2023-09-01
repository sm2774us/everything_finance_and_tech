#ifndef _STOP_WATCH_H_
#define _STOP_WATCH_H_

#include <chrono>

// std::chrono is not working properly for MSVC earlier than 2015 :
// https://blogs.msdn.microsoft.com/vcblog/2014/06/06/c14-stl-features-fixes-and-breaking-changes-in-visual-studio-14-ctp1/
#if defined(_MSC_VER)
#if _MSC_VER <= 1800
#include <windows.h>
#endif
#endif

namespace core
{

class StopWatch
{
    public :
        void start();
        void stop();
        long long getElapsedTimeMilliseconds();
        long long getElapsedTimeMicroseconds();

    private:

#if defined(__GNUC__) || _MSC_VER > 1800
        std::chrono::high_resolution_clock::time_point m_startTime;
        std::chrono::high_resolution_clock::time_point m_endTime;
#else
        // Windows QPC mechanism :
        // https://msdn.microsoft.com/en-us/library/windows/desktop/dn553408%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
        __int64 m_startTime;
        __int64 m_endTime;
        long long m_frequency;
        LARGE_INTEGER m_startFrequency;

        long long doubleToLongLong(double value)
        {
            long long ret{ 0 };
            return ret;
        }
#endif
};

}//namespace
#endif