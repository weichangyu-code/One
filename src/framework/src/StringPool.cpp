#include "StringPool.h"
#include <string.h>
#include "ObjectPool.h"

namespace One
{
    StringPool g_stringPool;
    
    StringPool::StringPool()
    {
    }
        
    void StringPool::setStringArray(const char** stringArray, unsigned int size)
    {
        this->_stringArray = stringArray;
        this->_pool = new String*[size + 1];
        memset(this->_pool, 0, (size + 1) * sizeof(String*));
    }

    Reference<String> StringPool::getString(int index)
    {
        String*& string = _pool[index];
        if (string == nullptr)
        {
            const char* str = _stringArray[index];
            string = String::createString(strlen(str)).detach();
            string->setData(str);
        }
        return Reference<String>(string, false, true);
    }

} // namespace One

