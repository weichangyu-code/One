#include "ObjectPool.h"
#include <string.h>

namespace One
{
    ObjectPool g_objectPool;

    ObjectPool::ObjectPool()
    {

    }
    
    Object* ObjectPool::createObject(unsigned int size)
    {
        Object* obj = (Object*)malloc(size);
        return obj;
    }

    void ObjectPool::freeObject(Object* obj)
    {
        CALL_DESTRUCT(obj, Object);
        free(obj);
    }
        
    void* ObjectPool::malloc(unsigned int size)
    {
        //4对齐
        size = (size + 3) / 4 * 4;
        void* mem = new char[size];
        memset(mem, 0, size);
        return mem;
    }
        
    void ObjectPool::free(void* mem)
    {
        delete (char*)mem;
    }
}
