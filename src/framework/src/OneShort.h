#pragma once
#include "OneBaseType.h"

namespace One
{
    class Short : public BaseType
    {
    public:
        Short()
        {

        }

        Short(OneShort value)
        {
            _value = value;
        }

        OneShort value()
        {
            return _value;
        }

        static Reference<Short> valueOf(OneShort value);
        virtual Reference<String> toString();

    protected:
        OneShort _value = false;
    };

    
} // namespace One
