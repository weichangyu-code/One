#pragma once
#include "../rule/RuleBlock.h"
#include "../lex/LexElement.h"
#include "../lex/LexAnalyzer.h"

class CodeAnalyzer
{
public:
    CodeAnalyzer(RuleBlock* block)
    {
        this->block = block;
    }

	list<LexElement> analysis(const string& str);

protected:
    RuleBlock* block;
};