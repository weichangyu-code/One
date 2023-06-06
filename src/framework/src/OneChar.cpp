#include "OneChar.h"
#include "ObjectPool.h"
#include "OneString.h"

namespace One
{
    
    Reference<Char> Char::valueOf(OneChar value)
    {
        Char* obj = g_objectPool.createObjectT<Char>();
        obj->_value = value;
        return Reference<Char>(obj, true, false);
    }
    
    Reference<String> Char::toString()
    {
        return String::valueOf(_value);
    }


} // namespace One
