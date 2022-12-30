#pragma once
#include "SyntaxBase.h"

class SyntaxVarDef;
class SyntaxBlock;
class SyntaxCatchBlock : public SyntaxBase
{
public:
    SyntaxCatchBlock(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    SyntaxVarDef* varDef = nullptr;
    SyntaxBlock* block = nullptr;
};

class SyntaxTryCatch : public SyntaxBase
{
public:
    SyntaxTryCatch(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    SyntaxBlock* tryBlock = nullptr;
    std::list<SyntaxCatchBlock*> catchBlocks;
    //SyntaxBlock* finalBlock = nullptr;            //不支撑final
};
