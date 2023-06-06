#pragma once
#include "OneBaseType.h"

namespace One
{
    class Float : public BaseType
    {
    public:
        Float()
        {

        }

        Float(OneFloat value)
        {
            _value = value;
        }

        OneFloat value()
        {
            return _value;
        }

        static Reference<Float> valueOf(OneFloat value);
        virtual Reference<String> toString();

    protected:
        OneFloat _value = false;
    };

    
} // namespace One
