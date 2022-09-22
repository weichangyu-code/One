#include "OneObject.h"
#include "ObjectPool.h"
#include "engine/Engine.h"
#include "OneClass.h"
#include "OneString.h"
#include "PointerUtils.h"
using namespace OneCommon;

namespace One
{
    void Object::__acquire__(bool inner)
    {
        if (this == nullptr)
        {
            return;
        }

        if (inner)
        {
            __innerRefNum__++;
        }
        else
        {
            __refNum__++;
        }
    }

    void Object::__release__(bool inner)
    {
        if (this == nullptr)
        {
            return;
        }

        if (inner)
        {
            if (--__innerRefNum__ == 0)
            {
                g_objectPool.freeObject(this);
            }
        }
        else
        {
            if (--__refNum__ == 0)
            {
                // 外部引用已结束，假析构
                __destroy__();
                __release__(true);
            }
        }
    }

    void Object::__destroy__()
    {
        if (this == nullptr)
        {
            return;
        }

        if (!(__flag__ & FLAG_DESTRUCT))
        {
            __flag__ |= FLAG_DESTRUCT;
            __destruct__();
        }
    }
        
    void Object::setLastError(int err)
    {
        OneCoroutine::Engine::getCurCoroutine()->setErrorCode(err);
    }
    
    int Object::getLastError()
    {
        return OneCoroutine::Engine::getCurCoroutine()->getErrorCode();
    }
        
    Reference<Class> Object::getClass()
    {
        return Reference<Class>(__class__, false, true);
    }
    void Object::initClass(Class* clazz)
    {
        __class__ = clazz;
    }
    
    Reference<String> Object::toString()
    {
        return String::createString("");
    }
        
    bool Object::equal(Object* obj)
    {
        return this == obj;
    }
        
    int Object::compare(Object* obj)
    {
        return PointerUtils::compare(this, obj);
    }
} // namespace One
