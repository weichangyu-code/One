#pragma once
#include "OneObject.h"
#include <vector>
#include <string>
using namespace std;

namespace One
{
    class Class;
    class Package : public Object
    {
    public:
        Package(Package* parent)
        {
            this->parent = parent;
        }
    
    public:
        string name;
        Package* parent = nullptr;
        vector<Package*> packages;
        vector<Class*> classes;
    };
} // namespace One
