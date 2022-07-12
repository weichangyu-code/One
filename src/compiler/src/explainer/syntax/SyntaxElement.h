#pragma once
#include "SyntaxBase.h"

class SyntaxSentence;
class SyntaxBlock;
class SyntaxClass;
class SyntaxIfBlock;
class SyntaxElement : public SyntaxBase
{
public:
    SyntaxElement(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    enum
    {
        SENTENCE,
        CODEBLOCK,
        IFBLOCK,
        FORBLOCK,
        WHILEBLOCK,
    };
    int type = SENTENCE;
    union
    {
        SyntaxSentence* sentence = nullptr;
        SyntaxBlock* block;
        SyntaxIfBlock* ifBlock;
    };
    
};
