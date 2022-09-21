#pragma once
#include "Common.h"
#include "Reference.h"

/**
 * 对象回收方式，通过引用索引和遍历算法
 * 
 * 
 */

namespace One
{
    class Class;
    class String;
    class Object
    {
        friend class MetaManager;
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

        //类
        Reference<Class> getClass();
        void initClass(Class* clazz);

        virtual Reference<String> toString();
        //virtual Reference<Object> clone();
        //virtual bool equal(Object* obj);

    protected:
        Class* __class__;           //在构造前被填充，不需要初始化
        enum
        {
            FLAG_DESTRUCT = 0x1
        };
        int __flag__ = 0;           //存放一些标志位
        int __refNum__ = 1;
        int __innerRefNum__ = 1;
    };

} // namespace One

#include "OneInterface.h"
#include "TemplateType.h"
#include "Pointer.inl"
#include "Reference.inl"
