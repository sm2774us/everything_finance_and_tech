#ifndef _POOL_H_
#define _POOL_H_

#include <vector>
#include <cstddef>
#include <cassert>
#include <algorithm>

namespace core
{

// Not thread safe
template<typename T, std::size_t initialCapacity, bool doubleSizeWhenGrowing=true>
class Pool
{
public:

    Pool() : m_doubleSizeWhenGrowing(doubleSizeWhenGrowing)
    {
        m_objects.reserve(initialCapacity);
        m_flags.reserve(initialCapacity);
        grow(initialCapacity);
    }

    int addObject(T t)
    {
        int ret{ -1 };

        auto newObjectIndex = getNonUsedObjectIndex();

        if (newObjectIndex == -1 && m_doubleSizeWhenGrowing == false)
        {
            PoolNode node;
            node.set(t);

            m_objects.emplace_back(node);
            m_flags.push_back(true);
            ret = getNumObjects() - 1;

            return ret;
        }
        else if (newObjectIndex == -1 && m_doubleSizeWhenGrowing == true)
        {
            auto currentNumOfObjects = getNumObjects();
            grow(currentNumOfObjects);
            newObjectIndex = currentNumOfObjects;

            m_objects[newObjectIndex].set(t);
            m_flags[newObjectIndex] = true;
            ret = newObjectIndex;

            return ret;
        }

        m_objects[newObjectIndex].set(t);
        m_flags[newObjectIndex] = true;
        ret = newObjectIndex;

        return ret;
    }

    void releaseObject(std::size_t objectIndex)
    {
        assert(objectIndex >= 0 && objectIndex < getNumObjects());
        m_flags[objectIndex] = false;
    }

    T* getObject(std::size_t objectIndex)
    {
        assert(objectIndex >= 0 && objectIndex < getNumObjects());
        T* ret = &(m_objects[objectIndex]);
        return ret;
    }

    std::size_t getNumObjects()
    {
        return m_objects.size();
    }

    std::size_t getNumOfUsedObjects()
    {
        return std::count_if(m_flags.begin(), m_flags.end(), [](bool b){return b == true; });
    }

private:
    bool m_doubleSizeWhenGrowing;
    std::vector<T> m_objects;
    std::vector<bool> m_flags;

    void grow(std::size_t numberObjects)
    {
        for (auto i = 0; i < initialCapacity; i++)
        {
            PoolNode node;
            m_objects.emplace_back(node);
            m_flags.push_back(false);
        }
    }

    std::size_t getNonUsedObjectIndex()
    {
        if (getNumObjects() == 0)
        {
            return -1;
        }

        std::size_t counter{ 0 };

        for (auto flag : m_flags)
        {
            if (m_flags[counter] == false)
            {
                return counter;
            }
            ++counter;
        }

        return -1;
    }
};

}//namespace

#endif