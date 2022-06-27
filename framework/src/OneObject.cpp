#include "OneObject.h"
#include "ObjectPool.h"

namespace One
{
    void Object::acquire(bool inner)
    {
        if (inner)
        {
            innerRefNum++;
        }
        else
        {
            refNum++;
        }
    }

    void Object::release(bool inner)
    {
        if (inner)
        {
            innerRefNum--;
            if (innerRefNum == 0)
            {
                g_objectPool.freeObject(this);
            }
        }
        else
        {
            refNum--;
            if (refNum == 0)
            {
                // 外部引用已结束，假析构
                __destruct__();
                release(true);
            }
        }
    } 
} // namespace One
