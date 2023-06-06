#pragma once
#include "OneBaseType.h"

namespace One
{
    class Long : public BaseType
    {
    public:
        Long()
        {

        }

        Long(OneLong value)
        {
            _value = value;
        }

        OneLong value()
        {
            return _value;
        }

        static Reference<Long> valueOf(OneLong value);
        virtual Reference<String> toString();

    protected:
        OneLong _value = false;
    };

    
} // namespace One
