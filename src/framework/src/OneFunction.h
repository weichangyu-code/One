#pragma once
#include "OneObject.h"
#include <vector>
#include <string>
using namespace std;

namespace One
{
    class Class;
    class Function : public Object
    {
    public:
        Function(Class* clazz)
        {
            this->clazz = clazz;
        }
    
    public:
        string name;
        Class* clazz = nullptr;
    };
} // namespace One
