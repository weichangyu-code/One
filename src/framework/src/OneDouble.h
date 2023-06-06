#pragma once
#include "OneBaseType.h"

namespace One
{
    class Double : public BaseType
    {
    public:
        Double()
        {

        }

        Double(OneDouble value)
        {
            _value = value;
        }

        OneDouble value()
        {
            return _value;
        }

        static Reference<Double> valueOf(OneDouble value);
        virtual Reference<String> toString();

    protected:
        OneDouble _value = false;
    };

    
} // namespace One
