﻿#include "OneObject.h"
#include "ObjectPool.h"
#include "engine/Engine.h"
#include "engine/CoStdOut.h"
#include "OneClass.h"
#include "OneString.h"
#include "PointerUtils.h"
using namespace OneCommon;
using namespace OneCoroutine;

namespace One
{
    void Object::__acquire__(bool owner)
    {
        if (this == nullptr)
        {
            return;
        }

        if (owner)
        {
            __refNum__++;
        }
        else
        {
            __innerRefNum__++;
        }
    }

    void Object::__release__(bool owner)
    {
        if (this == nullptr)
        {
            return;
        }

        if (owner)
        {
            if (--__refNum__ == 0)
            {
                // 外部引用已结束，假析构
                __destroy__();
                __release__(true);
            }
        }
        else
        {
            if (--__innerRefNum__ == 0)
            {
                g_objectPool.freeObject(this);
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

            bool catchException = false;
            try
            {
                __destruct__();
            }
            catch(...)
            {
                //捕获析构异常，避免其他资源无法释放。C++不允许析构抛出异常。
                //不能在异常里面协程切换
                catchException = true;
            }
            if (catchException)
            {
                CoStdOut::print("__destruct__: catch exception\n");
            }
            __clearVar__();
        }
    }
        
    void Object::setLastError(int err)
    {
        Engine::getCurCoroutine()->setErrorCode(err);
    }
    
    int Object::getLastError()
    {
        return Engine::getCurCoroutine()->getErrorCode();
    }
        
    Reference<Class> Object::getClass()
    {
        return Reference<Class>(__class__, true, true);
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
