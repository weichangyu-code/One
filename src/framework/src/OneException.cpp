#include "OneException.h"
#include "ObjectPool.h"

namespace One
{
    void throwNullPointExeption()
    {
        throw g_objectPool.createObjectR<NullPointerException>();
    }
} // namespace One
