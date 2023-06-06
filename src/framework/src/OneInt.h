#pragma once
#include "OneBaseType.h"

namespace One
{
    class Int : public BaseType
    {
    public:
        Int()
        {

        }

        Int(OneInt value)
        {
            _value = value;
        }

        OneInt value()
        {
            return _value;
        }

        static Reference<Int> valueOf(OneInt value);
        virtual Reference<String> toString();

    protected:
        OneInt _value = false;
    };

    
} // namespace One
