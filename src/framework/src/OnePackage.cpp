#include "OnePackage.h"
#include "OneClass.h"

namespace One
{
    
    Package* Package::getPackage(const string& name)
    {
        for (auto& sub : packages)
        {
            if (sub->name == name)
            {
                return sub;
            }
        }
        return nullptr;
    }

    Class* Package::getClass(const string& name)
    {
        for (auto& clazz : classes)
        {
            if (clazz->name == name)
            {
                return clazz;
            }
        }
        return nullptr;
    }
} // namespace One
