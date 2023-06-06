#pragma once
#include "OneBaseType.h"

namespace One
{
    class Char : public BaseType
    {
    public:
        Char()
        {

        }

        Char(OneChar value)
        {
            _value = value;
        }

        OneChar value()
        {
            return _value;
        }

        static Reference<Char> valueOf(OneChar value);
        virtual Reference<String> toString();

    protected:
        OneChar _value = false;
    };

    
} // namespace One
