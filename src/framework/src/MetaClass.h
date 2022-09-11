#pragma once
#include <vector>
#include <string>
using namespace std;

namespace One
{
    class MetaPackage;
    class MetaClass
    {
    public:
        MetaClass(MetaPackage* package)
        {
            this->package = package;
        }
    
    public:
        int id = 0;
        string name;
        MetaPackage* package;
    };
} // namespace One
