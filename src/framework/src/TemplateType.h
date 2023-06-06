#pragma once
#include "Type.h"

namespace One
{
    ///////////////////////////////////////////////////////////////////////////////////
    //判断是否是基础类型
    template<typename T>
    class IsBaseType
    {
    public:
        const static bool value = false;
    };

    template<>
    class IsBaseType<OneBool>
    {
    public:
        const static bool value = true;
    };

    template<>
    class IsBaseType<OneChar>
    {
    public:
        const static bool value = true;
    };

    template<>
    class IsBaseType<OneShort>
    {
    public:
        const static bool value = true;
    };

    template<>
    class IsBaseType<OneInt>
    {
    public:
        const static bool value = true;
    };

    template<>
    class IsBaseType<OneLong>
    {
    public:
        const static bool value = true;
    };

    template<>
    class IsBaseType<OneFloat>
    {
    public:
        const static bool value = true;
    };

    template<>
    class IsBaseType<OneDouble>
    {
    public:
        const static bool value = true;
    };

    ///////////////////////////////////////////////////////////////////////////////////

    template<typename T, bool isBaseType = IsBaseType<T>::value>
    class TemplateType
    {
    public:
        typedef T Type;             //类型
        typedef T FuncParamType;    //对应函数参数类型
        typedef T VarType;          //成员变量类型
    };

    template<typename T>
    class TemplateType<T, false>
    {
    public:
        typedef T Type;
        typedef Pointer<T> FuncParamType;
        typedef Reference<T> VarType;
    };

    ///////////////////////////////////////////////////////////////////////////////////
    template<typename T, bool isBaseType = IsBaseType<T>::value>
    class DefaultValue
    {
    public:
        static typename TemplateType<T>::VarType value()
        {
            return nullptr;
        }
    };

    template<typename T>
    class DefaultValue<T, true>
    {
    public:
        static typename TemplateType<T>::VarType value()
        {
            return 0;
        }
    };
}
