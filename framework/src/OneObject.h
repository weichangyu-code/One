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
        void acquire(bool inner);
        void release(bool inner);

    public:
        RunClass* runClass = nullptr;
        int flag = 0;         //存放一些标志位
        int refNum = 1;
        int innerRefNum = 1;
    };
} // namespace One

#include "OneInterface.h"
#include "Reference.h"
#include "TemplateType.h"
