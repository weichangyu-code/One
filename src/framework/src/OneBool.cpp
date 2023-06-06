#include "OneBool.h"
#include "ObjectPool.h"
#include "OneString.h"

namespace One
{
    
    Reference<Bool> Bool::valueOf(bool value)
    {
        Bool* obj = g_objectPool.createObjectT<Bool>();
        obj->_value = value;
        return Reference<Bool>(obj, true, false);
    }
    
    Reference<String> Bool::toString()
    {
        return String::createString(this->_value ? OSTR "true" : OSTR "false");
    }


} // namespace One
