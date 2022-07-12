#pragma once
#include "SyntaxBase.h"

class SyntaxBlock;
class SyntaxExp;

class SyntaxIfItem : public SyntaxBase
{
public:
    SyntaxIfItem(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    SyntaxExp* exp = nullptr;       //else 的话，为null
    SyntaxBlock* block = nullptr;
};

class SyntaxIfBlock : public SyntaxBase
{
public:
    SyntaxIfBlock(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    list<SyntaxIfItem*> items;
};

