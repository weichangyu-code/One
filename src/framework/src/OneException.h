#pragma once
#include "OneObject.h"

namespace One
{
    class Exception : public Object
    {
    public:
        Exception()
        {

        }
    };

    class NullPointerException : public Exception
    {
    public:
        NullPointerException()
        {

        }
    };

} // namespace One




