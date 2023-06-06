#pragma once
#include "OneObject.h"

namespace One
{
    template<typename T>
    class Iterator : public Interface
    {
    public:
        Iterator(Object* obj) : Interface(obj)
        {}
    public:
        virtual OneBool hasNext() = 0;
        virtual typename TemplateType<T>::VarType next() = 0;
    };
}
