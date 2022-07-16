#pragma once

/**
 * 对象回收方式，通过引用索引和遍历算法
 * 
 * 
 */

namespace One
{
    class RunClass;
    class Object
    {
    public:
        Object(){}
        virtual ~Object(){}
        virtual void __destruct__(){}    //假析构

    public:
        void __acquire__(bool inner);
        void __release__(bool inner);
        void __destroy__();              //强制析构

    protected:
        RunClass* __runClass__ = nullptr;
        enum
        {
            FLAG_DESTRUCT = 0x1
        };
        int __flag__ = 0;         //存放一些标志位
        int __refNum__ = 1;
        int __innerRefNum__ = 1;
    };
} // namespace One

#include "OneInterface.h"
#include "Reference.h"
#include "TemplateType.h"
