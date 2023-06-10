#pragma once
#include "SyntaxBase.h"
#include "../common/Instruct.h"
#include "SyntaxData.h"

class SyntaxFunc;
class SyntaxType;
class SyntaxExp;
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
        void* p = nullptr;
        SyntaxType* type;                   //cmd为TYPE、NEW有效
        SyntaxVarDef* varDef;               //cmd为VARDEF有效
        //SyntaxVar* func;                  //
        SyntaxExp* func;                    //cmd为CALL有效
    };
    
    std::list<SyntaxData> params;
};
