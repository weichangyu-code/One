#pragma once
#include "OneObject.h"
#include <vector>
#include <string>
using namespace std;

namespace One
{
    class Class;
    class Field : public Object
    {
    public:
        Field(Class* clazz)
        {
            this->clazz = clazz;
        }
    
    public:
        string name;
        Class* clazz = nullptr;
    };
} // namespace One
