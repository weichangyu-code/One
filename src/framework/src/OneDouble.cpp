#include "OneDouble.h"
#include "ObjectPool.h"
#include "OneString.h"

namespace One
{
    
    Reference<Double> Double::valueOf(OneDouble value)
    {
        Double* obj = g_objectPool.createObjectT<Double>();
        obj->_value = value;
        return Reference<Double>(obj, true, false);
    }
    
    Reference<String> Double::toString()
    {
        return String::valueOf(_value);
    }


} // namespace One
