#ifndef _SINGLETON_STATIC_H_
#define _SINGLETON_STATIC_H_

#include <core/noncopyable.h>

/*
    Scott Meyers` singleton
    In C++11 statics are initialized in a thread safe way

    Note that , the order of initialisation for statics in different translation units
    is undefined , so this could be problematic if more singletons used in the project

    Note about MSVC : In MSVC using static is thread safe for only VS2015:
    https://msdn.microsoft.com/en-us/library/hh567368.aspx?f=255&MSPPError=-2147217396
*/

#if defined(_MSC_VER)
#if _MSC_VER<1900
#error "Thread safe initialisation of static variables is not supported by a pre-2015 MSVC compiler."
#endif
#endif

namespace core
{

template<typename T>
class SingletonStatic : public core::NonCopyable
{
public:
    static T& getInstance()
    {
        static T single_instance;
        return single_instance;
    }
};

}
#endif