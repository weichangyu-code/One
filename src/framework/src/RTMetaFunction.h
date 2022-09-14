#pragma once
#include <vector>
#include <string>
using namespace std;

namespace One
{
    class RTMetaClass;
    class RTMetaFunction
    {
    public:
        RTMetaFunction(RTMetaClass* clazz)
        {
            this->clazz = clazz;
        }
    
    public:
        string name;
        RTMetaClass* clazz;
    };
} // namespace One
