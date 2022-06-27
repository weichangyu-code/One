#pragma once
#include "Common.h"

class CppBase;
class CppContainer
{
public:
    CppContainer();
    ~CppContainer();
    
//对象管理
public:
    void addCppObject(CppBase* obj);
protected:
    list<CppBase*> cppObjs;
};
