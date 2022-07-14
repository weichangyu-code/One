#pragma once
#include "SyntaxBase.h"

class SyntaxBlock;
class SyntaxExp;
class SyntaxVarDef;

class SyntaxForBlock : public SyntaxBase
{
public:
    SyntaxForBlock(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    enum
    {
        FOR,
        FOR_RANGE,
        FOR_EACH,
    };
    int type = FOR;
    SyntaxVarDef* varDef = nullptr;
    SyntaxExp* exp[3] = {nullptr};
    SyntaxBlock* block = nullptr;
};
