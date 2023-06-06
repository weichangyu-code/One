#include "OneInt.h"
#include "ObjectPool.h"
#include "OneString.h"

namespace One
{
    
    Reference<Int> Int::valueOf(OneInt value)
    {
        Int* obj = g_objectPool.createObjectT<Int>();
        obj->_value = value;
        return Reference<Int>(obj, true, false);
    }
    
    Reference<String> Int::toString()
    {
        return String::valueOf(_value);
    }


} // namespace One
