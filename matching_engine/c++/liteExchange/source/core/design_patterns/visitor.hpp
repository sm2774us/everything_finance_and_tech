#ifndef _VISITOR_H_
#define _VISITOR_H_

namespace core
{

// Base template class for visitors
template<typename T>
class Visitor
{
    public:
        virtual void visit(T& element) = 0;
};

// Base template class for visitables
template<typename T>
class Visitable
{
    public:
        virtual void accept(Visitor<T>& v) = 0;
};

}//namespace

#endif