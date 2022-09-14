#pragma once
#include <vector>
#include <string>
using namespace std;

namespace One
{
    class RTMetaFunction;
    class RTMetaField;
    class RTMetaPackage;
    class RTMetaClass
    {
    public:
        RTMetaClass()
        {
        }
    
    public:
        int id = 0;
        string name;

        RTMetaPackage* package = nullptr;
        RTMetaClass* outerClass = nullptr;

        vector<RTMetaClass*> parents;

        vector<RTMetaClass*> inners;
        vector<RTMetaFunction*> funcs;
        vector<RTMetaField*> fields;
    };
} // namespace One
