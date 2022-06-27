#pragma once
#include "SyntaxBase.h"

class SyntaxExp;
class SyntaxSentence : public SyntaxBase
{
public:
    SyntaxSentence(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    SyntaxExp* exp = nullptr;
};
