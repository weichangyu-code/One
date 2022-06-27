#pragma once
#include "MetaBase.h"
#include "Common.h"
#include "../common/Type.h"
#include "MetaType.h"

class MetaConst : public MetaBase
{
public:
    MetaConst(MetaContainer* metaContainer, SyntaxBase* syntaxObj)
        :MetaBase(metaContainer, syntaxObj)
    {
    }
    
    void setCharValue(signed char cvalue)
    {
        this->cvalue = cvalue;
        this->type = DT_CHAR;
    }

    void setBoolValue(bool bvalue)
    {
        this->bvalue = bvalue;
        this->type = DT_BOOL;
    }

    void setIntValue(signed int ivalue)
    {
        this->ivalue = ivalue;
        this->type = DT_INT;
    }

    void setLongValue(signed long long lvalue)
    {
        this->lvalue = lvalue;
        this->type = DT_LONG;
    }

    void setFloatValue(float fvalue)
    {
        this->fvalue = fvalue;
        this->type = DT_FLOAT;
    }

    void setDoubleValue(double dvalue)
    {
        this->dvalue = dvalue;
        this->type = DT_DOUBLE;
    }

    void setStringValue(const string& str)
    {
        this->strvalue = str.c_str();
        this->type = DT_STRING;
    }

    void setRefNull()
    {
        this->type = DT_CLASS;
        this->ptrvalue = nullptr;
    }

    bool isRefNull()
    {
        return this->type == DT_CLASS && this->ptrvalue == nullptr;
    }

    MetaType getType();

public:
    int type = DT_NONE;
    union
    {
        bool bvalue;
        signed char cvalue;
        signed int ivalue;
        signed long long lvalue;
        float fvalue;
        double dvalue;
        const char* strvalue;
        void* ptrvalue = nullptr;
    };
    string remark;
};
