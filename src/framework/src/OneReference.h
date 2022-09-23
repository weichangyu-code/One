#pragma once
#include "OneObject.h"

namespace One
{
    template<class T>
    class ReferenceT : public Object
    {
    public:
        ReferenceT()
        {}

        static Reference<T> toOwner(T* obj)
        {
            return Reference<T>(obj, true, true);
        }

        static Reference<T> toInner(T* obj)
        {
            return Reference<T>(obj, false, true);
        }
    };
} // namespace One

