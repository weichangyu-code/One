#pragma once
#include <vector>
#include <string>
using namespace std;

namespace One
{
    class RTMetaClass;
    class RTMetaField
    {
    public:
        RTMetaField(RTMetaClass* clazz)
        {
            this->clazz = clazz;
        }
    
    public:
        string name;
        RTMetaClass* clazz;
    };
} // namespace One
