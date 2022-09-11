#pragma once
#include <vector>
#include <string>
using namespace std;

namespace One
{
    class MetaClass;
    class MetaPackage
    {
    public:
        MetaPackage(MetaPackage* parent)
        {
            this->parent = parent;
        }
    
    public:
        string name;
        MetaPackage* parent = nullptr;
        vector<MetaPackage*> packages;
        vector<MetaClass*> classes;
    };
} // namespace One
