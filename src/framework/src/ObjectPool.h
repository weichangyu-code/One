#pragma once
#include "OneString.h"
#include "OneArray.h"
#include <new>

#define CALL_CONSTRUCT(p, TYPE, ...)	{::new ((void*)(p)) TYPE (__VA_ARGS__);}
#define CALL_DESTRUCT(p, TYPE)			{typedef TYPE my_type;void* p2 = p;((my_type*)p2)->~my_type();}

namespace One
{
    class ObjectPool
    {
    public:
        ObjectPool();

    public:
        Object* createObject(unsigned int size);        //内部没调用构造函数
        void freeObject(Object* obj);                   //内部调用析构函数

        Reference<String> createString(unsigned int length)
        {
            String* str = (String*)createObject(sizeof(String) + length);
            CALL_CONSTRUCT(str, String);
            str->_length = length;
            return Reference<String>(str, false, false);
        }

        template<class T>
        Reference<Array<T>> createArray(unsigned int length)
        {
            Array<T>* arr = (Array<T>*)createObject(sizeof(Array<T>) + length * sizeof(T));
            CALL_CONSTRUCT(arr, Array<T>);
            arr->_length = length;
            return Reference<Array<T>>(arr, false, false);
        }

    protected:
        void* malloc(unsigned int size);
        void  free(void* mem);
    };

    extern ObjectPool g_objectPool;
}
