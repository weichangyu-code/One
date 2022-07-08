#pragma once
#include "SyntaxBase.h"

class SyntaxSentence;
class SyntaxBlock;
class SyntaxClass;
class SyntaxElement : public SyntaxBase
{
public:
    SyntaxElement(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    SyntaxSentence* sentence = nullptr;
    SyntaxBlock* block = nullptr;
    SyntaxClass* clazz = nullptr;
};
