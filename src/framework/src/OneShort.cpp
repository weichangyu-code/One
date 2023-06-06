#include "OneShort.h"
#include "ObjectPool.h"
#include "OneString.h"

namespace One
{
    
    Reference<Short> Short::valueOf(OneShort value)
    {
        Short* obj = g_objectPool.createObjectT<Short>();
        obj->_value = value;
        return Reference<Short>(obj, true, false);
    }
    
    Reference<String> Short::toString()
    {
        return String::valueOf(_value);
    }


} // namespace One
