﻿#pragma once
#include "Common.h"
#include "MetaContainer.h"

/**
 * 对象回收方式，通过引用索引和遍历算法
 * 
 * 
 */

namespace One
{
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

        //错误码
        static void setLastError(int err);
        static int  getLastError();

    protected:
        MetaClass* __metaClass__;       //在构造前被填充，不需要初始化
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
