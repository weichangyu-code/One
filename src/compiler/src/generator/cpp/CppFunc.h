#pragma once
#include "Common.h"
#include "CppBase.h"

class MetaFunc;
class CppFunc : public CppBase
{
public:
    CppFunc(MetaFunc* metaFunc, CppContainer* cppContainer);

    static CppFunc* getCppFunc(MetaFunc* metaFunc);

public:
    MetaFunc* metaFunc = nullptr;

    //编译C++的函数名
    string cppName;
};