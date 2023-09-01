#ifndef _NONCOPYABLE_
#define _NONCOPYABLE_

namespace core
{

class NonCopyable
{
    protected:
        NonCopyable() = default;
        ~NonCopyable() = default;
    private :
        NonCopyable( const NonCopyable& ) = delete;
        NonCopyable& operator=( const NonCopyable& ) = delete;
};

}

#endif