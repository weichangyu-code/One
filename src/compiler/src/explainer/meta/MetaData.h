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
    bool isInstructTmp();

    MetaType getType();

    bool operator == (const MetaData& r);

public:
    enum
    {
        NONE,
        CONST,
        VARREF,
        INSTRUCT,
        VAR,
        MEMBER,             //作为对象成员，var有效
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
