#pragma once
#include "OneObject.h"
#include "OneIterator.h"

namespace One
{
    template<typename T>
    class Iterable : public Interface
    {
    public:
        Iterable(Object* obj) : Interface(obj)
        {}
    public:
        virtual Reference<Iterator<T>> iterator() = 0;
    };
}
