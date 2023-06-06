#include "OneClass.h"
#include "OneString.h"

namespace One
{
    OneBool Class::isBaseOf(Class* clazz)
    {
        if (this == nullptr || clazz == nullptr)
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
        return String::createString(OSTR name.c_str());
    }
    
} // namespace One
