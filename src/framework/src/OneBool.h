#pragma once
#include "OneBaseType.h"

namespace One
{
    class Bool : public BaseType
    {
    public:
        Bool()
        {

        }

        Bool(OneBool value)
        {
            _value = value;
        }

        OneBool value()
        {
            return _value;
        }

        static Reference<Bool> valueOf(OneBool value);
        virtual Reference<String> toString();

    protected:
        OneBool _value = false;
    };

    
} // namespace One
