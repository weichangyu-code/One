#include "OneLong.h"
#include "ObjectPool.h"
#include "OneString.h"

namespace One
{
    
    Reference<Long> Long::valueOf(OneLong value)
    {
        Long* obj = g_objectPool.createObjectT<Long>();
        obj->_value = value;
        return Reference<Long>(obj, true, false);
    }
    
    Reference<String> Long::toString()
    {
        return String::valueOf(_value);
    }


} // namespace One
