#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <vector>
#include <string>

namespace core
{

template <class T>
class Observer
{
    public:
        Observer() {}
        virtual void onEvent(const std::string& eventMessage) = 0;
};

template <class T>
class Observable
{
public:
    Observable() {}

    void attach(Observer<T> &observer)
    {
        m_observers.push_back(&observer);
    }

    void notify(const std::string& eventMessage)
    {
        for (auto observer : m_observers)
        {
            observer->onEvent(eventMessage);
        }
    }

private:
    std::vector<Observer<T> *> m_observers;
};

}//namespace

#endif