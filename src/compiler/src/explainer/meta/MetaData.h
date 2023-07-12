#pragma once
#include "Common.h"
#include "MetaType.h"

class MetaConst;
class MetaVarRef;
class MetaInstruct;
class MetaVariable;
class MetaData
{
public:
    MetaData();
    MetaData(int type, void* ptr);
    MetaData(MetaConst* const_);
    MetaData(MetaVarRef* varRef);
    MetaData(MetaInstruct* instructTmp);
    MetaData(MetaVariable* var);

    void setData(int type, void* ptr);
    void setData(MetaConst* const_);
    void setData(MetaVarRef* varRef);
    void setData(MetaInstruct* instructTmp);
    void setData(MetaVariable* var);
    void clear();

    bool isNone();
    bool isConst();
    bool isVariable();
    bool isConstVariable();
    bool isInstructTmp();

    MetaType getType() const;

    bool operator == (const MetaData& r);

    //获取当前数据是不是调用某个函数返回值
    MetaInstruct* checkFuncReturn(const char* name);

public:
    enum
    {
        NONE,
        CONST,
        VARREF,
        INSTRUCT,
        VAR,
    };
    int type = NONE;
    union
    {
        MetaConst* const_ = nullptr;
        MetaVarRef* varRef;
        MetaInstruct* instructTmp;
        MetaVariable* var;
    };
};
