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
    bool dowhile = false;
    SyntaxExp* exp = nullptr;
    SyntaxBlock* block = nullptr;
};
