#ifndef _MAKE_UNIQUE_HPP_
#define _MAKE_UNIQUE_HPP_

#include <memory>
#include <utility>
#include <type_traits>

namespace core
{


/*
 Perfect forwarding helps us to retain lvalue/rvalue property of a template function argument.
 Without using perfect forwarding, when you pass an rvalue as template function argument,
 it will be deduced to type value. The example below is a simple implementation of make_unique functionality for C++11.
 ( make_unique added in C++14 ).

 Perfect forwarding is performed in two steps: receive a forwarding reference (also known as universal reference), then forward it using std::forward.

 REFERENCE COLLAPSING RULES : http://thbecker.net/articles/rvalue_references/section_08.html

                        U& & => U&
                        U&& & => U&
                        U& && => U&
                        U&& && => U&&


            Therefore it is called also as "universal reference"

            std::move -> turns into rvalue
            std::forward -> turns into WHATEVER it is supposed to be

 If you don`t use std::forward , the same constructor will be called.
 But with std::forward, different constructors will be called :
*/
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


}

#endif