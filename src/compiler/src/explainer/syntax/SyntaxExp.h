#pragma once
#include "SyntaxBase.h"
#include "SyntaxData.h"
#include "SyntaxMulti.h"

class SyntaxInstruct;
class SyntaxExp : public SyntaxBase
{
public:
    SyntaxExp(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

    void append(SyntaxExp* exp, bool start = false);
    void removeInstruct(SyntaxInstruct* instruct);

    static SyntaxExp* combine(SyntaxMulti<SyntaxExp*>* multiExp, bool addCommaInstruct, ExplainContext* context);

public:
    list<SyntaxInstruct*> instructs;
    SyntaxData ret;
};