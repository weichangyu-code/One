#pragma once
#include "SyntaxBase.h"

class SyntaxConst;
class SyntaxInstruct;
class SyntaxVar;
class SyntaxVarDef;
class SyntaxData
{
public:
    SyntaxData()
    {}
    SyntaxData(SyntaxConst* const_)
    {
        this->type = CONST;
        this->const_ = const_;
    }
    SyntaxData(SyntaxVar* var)
    {
        this->type = VAR;
        this->var = var;
    }
    SyntaxData(SyntaxVarDef* varDef)
    {
        this->type = VARDEF;
        this->varDef = varDef;
    }
    SyntaxData(SyntaxInstruct* instruct)
    {
        this->type = INSTRUCT;
        this->instruct = instruct;
    }
    
public:
    void setConst(SyntaxConst* const_)
    {
        this->type = CONST;
        this->const_ = const_;
    }
    void setVar(SyntaxVar* var)
    {
        this->type = VAR;
        this->var = var;
    }
    void setVarDef(SyntaxVarDef* varDef)
    {
        this->type = VARDEF;
        this->varDef = varDef;
    }
    void setInstruct(SyntaxInstruct* instruct)
    {
        this->type = INSTRUCT;
        this->instruct = instruct;
    }

public:
    enum
    {
        NONE,
        CONST,
        VAR,
        VARDEF,
        INSTRUCT,
    };

    int type = NONE;
    union
    {
        SyntaxConst* const_ = nullptr;
        SyntaxVar* var;
        SyntaxVarDef* varDef;
        SyntaxInstruct* instruct;
    };
};
