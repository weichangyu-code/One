#include "OneFloat.h"
#include "ObjectPool.h"
#include "OneString.h"

namespace One
{
    
    Reference<Float> Float::valueOf(OneFloat value)
    {
        Float* obj = g_objectPool.createObjectT<Float>();
        obj->_value = value;
        return Reference<Float>(obj, true, false);
    }
    
    Reference<String> Float::toString()
    {
        return String::valueOf(_value);
    }


} // namespace One
