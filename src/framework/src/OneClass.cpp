#include "OneClass.h"

namespace One
{
    bool Class::isBaseOf(Class* clazz)
    {
        if (clazz == nullptr)
        {
            return false;
        }
        if (clazz == this)
        {
            return true;
        }
        for (auto& parent : parents)
        {
            if (parent->isBaseOf(clazz))
            {
                return true;
            }
        }
        return false;
    }
    
} // namespace One
