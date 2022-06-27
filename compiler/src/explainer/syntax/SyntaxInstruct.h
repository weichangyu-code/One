#pragma once
#include "SyntaxBase.h"
#include "../common/Instruct.h"
#include "SyntaxData.h"

class SyntaxFunc;
class SyntaxType;
class SyntaxInstruct : public SyntaxBase
{
public:
    SyntaxInstruct(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    int cmd = NOP;

    union 
    {
        SyntaxVar* func = nullptr;          //cmd为CALL有效
        SyntaxType* type;                   //cmd为TYPE、NEW有效
        SyntaxInstruct* jump;               //cmd为JUMP有效
        SyntaxVarDef* varDef;               //cmd为VARDEF有效
    };
    
    std::list<SyntaxData> params;
};
