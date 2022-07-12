#pragma once
#include "SyntaxBase.h"

class SyntaxElement;
class SyntaxBlock : public SyntaxBase
{
public:
    SyntaxBlock(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    list<SyntaxElement*> elements;
};
