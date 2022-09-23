#pragma once
#include "OneObject.h"
#include "OneClass.h"
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

        template<typename T>
        T* createObjectT(unsigned int size)
        {
            T* p = (T*)createObject(size);
            p->initClass(ClassP<T>::getClass());
            CALL_CONSTRUCT(p, T);
            return p;
        }

        template<typename T>
        T* createObjectT()
        {
            return createObjectT<T>(sizeof(T));
        }

        template<typename T>
        Reference<T> createObjectR()
        {
            return Reference<T>(createObjectT<T>(), true, false);
        }

    protected:
        void* malloc(unsigned int size);
        void  free(void* mem);
    };

    extern ObjectPool g_objectPool;
}
