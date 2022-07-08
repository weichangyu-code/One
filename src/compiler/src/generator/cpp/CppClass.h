#pragma once
#include "Common.h"
#include "CppBase.h"

class MetaClass;
class CppClass : public CppBase
{
public:
    CppClass(MetaClass* metaClass, CppContainer* cppContainer);

    static CppClass* getCppClass(MetaClass* metaClass);
    static CppClass* getCppClass(MetaClass* metaClass, CppContainer* cppContainer);

    string getFactoryName();

    string getTemplateParamDefine();
    string getTemplateParamNames();
    string getTemplateParamImpl();

public:
    MetaClass* metaClass = nullptr;

    enum
    {
        STEP_READY,
        STEP_CPPNAME,
    };
    int step = STEP_READY;

    //C++的类名，和相对路径名。如果是原生的就不用生成
    string cppName;
    string cppPath;
    string cppHPath;
    
    bool cppNative = false;
    string cppNativeName;
    string cppNativeHPath;
};
