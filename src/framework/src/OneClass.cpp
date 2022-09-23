#include "OneClass.h"
#include "OneString.h"

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
        
    Reference<String> Class::toString()
    {
        return String::createString(name.c_str());
    }
    
} // namespace One
