#pragma once
#include "OneObject.h"
#include <vector>
#include <string>
using namespace std;

namespace One
{
    class Function;
    class Field;
    class Package;
    class Class : public Object
    {
    public:
        Class()
        {
        }

        bool isBaseOf(Class* clazz);
    
    public:
        int id = 0;
        string name;

        Package* package = nullptr;
        Class* outerClass = nullptr;

        vector<Class*> parents;

        vector<Class*> inners;
        vector<Function*> funcs;
        vector<Field*> fields;
    };
} // namespace One
