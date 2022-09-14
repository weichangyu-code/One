#pragma once
#include <vector>
#include <string>
using namespace std;

namespace One
{
    class RTMetaClass;
    class RTMetaPackage
    {
    public:
        RTMetaPackage(RTMetaPackage* parent)
        {
            this->parent = parent;
        }
    
    public:
        string name;
        RTMetaPackage* parent = nullptr;
        vector<RTMetaPackage*> packages;
        vector<RTMetaClass*> classes;
    };
} // namespace One
