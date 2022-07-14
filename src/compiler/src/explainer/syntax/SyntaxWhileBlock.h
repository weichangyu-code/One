#pragma once
#include "SyntaxBase.h"

class SyntaxBlock;
class SyntaxExp;

class SyntaxWhileBlock : public SyntaxBase
{
public:
    SyntaxWhileBlock(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    enum
    {
        WHILE,
        DOWHILE,
    };
    int type = WHILE;
    SyntaxExp* exp = nullptr;
    SyntaxBlock* block = nullptr;
};
