#pragma once
#include "Reference.h"

namespace One
{
    ///////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    class IsBaseType
    {
    public:
        const static bool value = false;
    };

    template<>
    class IsBaseType<bool>
    {
    public:
        const static bool value = true;
    };

    template<>
    class IsBaseType<signed char>
    {
    public:
        const static bool value = true;
    };

    template<>
    class IsBaseType<signed short>
    {
    public:
        const static bool value = true;
    };

    template<>
    class IsBaseType<signed int>
    {
    public:
        const static bool value = true;
    };

    template<>
    class IsBaseType<signed long long>
    {
    public:
        const static bool value = true;
    };

    template<>
    class IsBaseType<float>
    {
    public:
        const static bool value = true;
    };

    template<>
    class IsBaseType<double>
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


}
