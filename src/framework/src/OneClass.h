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
        int objectSize = 0;

        Package* package = nullptr;
        Class* outerClass = nullptr;
        Function* outerFunc = nullptr;

        vector<Class*> parents;
        struct ParentOffset
        {
            int id;
            int offset;
        };
        vector<ParentOffset> parentOffsets;         //用于类型转换

        vector<Class*> inners;
        vector<Function*> funcs;
        vector<Field*> fields;
    };

    //存储每个类的Class指针
    template<class T>
    class ClassP
    {
    public:
        static Class* getClass()
        {
            return clazz;
        }

    public:
        static Class* clazz;
    };
    template<class T>
    Class* ClassP<T>::clazz = nullptr;

} // namespace One