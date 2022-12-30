#include "ExceptionHelper.h"
#include "ObjectPool.h"

namespace One
{
    void throwException(Exception* e)
    {
        throw ExceptionBox(e);
    }

    void throwNullPointerException()
    {
        throwException(g_objectPool.createObjectR<NullPointerException>());
    }

    void throwOutOfArrayException()
    {
        throwException(g_objectPool.createObjectR<OutOfArrayException>());
    }
} // namespace One
