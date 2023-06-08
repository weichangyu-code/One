#pragma once
#include "Common.h"
#include "../common/Type.h"

class MetaTemplateParam;
class MetaClass;
class MetaType
{
public:
    static int stringToType(const string& type);
    static string typeToString(int type);
    static string typeToCppString(int type);

public:
    MetaType(int type = DT_NONE);
    MetaType(MetaClass* clazz);

    void setClass(MetaClass* clazz);
    void setType(int type);
    void setType(const string& type);
    void setBool();
    void setNull();

    bool isNone() const;
    bool isClass() const;
    bool isBaseType() const;
    bool isInteger() const;						    //是否是整数
    bool isFloat() const;
    bool isRealNumber() const;						//是不是实数
    bool isBool() const;
    bool isVoid() const;
    bool isNull() const;                            //null类型
    
    bool equal(const MetaType& r) const;
    int  compare(const MetaType& r) const;
    bool operator < (const MetaType& r) const;
    bool operator == (const MetaType& r) const;
    bool operator != (const MetaType& r) const;

public:
    int type = DT_NONE;             //string会转成class
    MetaClass* clazz = nullptr;
};
