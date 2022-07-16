#include "StringPool.h"
#include <string.h>
#include "ObjectPool.h"

namespace One
{
    StringPool g_stringPool;
    
    StringPool::StringPool()
    {
        pool = nullptr;
    }

    void StringPool::reserve(unsigned int size)
    {
        pool = new String*[size + 1];
        memset(pool, 0, (size + 1) * sizeof(String*));
    }
        
    void StringPool::addString(int index, const char* str)
    {
        String* obj = String::createString(strlen(str)).detach();
        obj->setData(str);
        pool[index] = obj;
    }

    Reference<String> StringPool::getString(int index)
    {
        return Reference<String>(pool[index], false, true);
    }

} // namespace One

