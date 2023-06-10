#pragma once
#include "SyntaxBase.h"
#include "SyntaxData.h"
#include "SyntaxMulti.h"

class SyntaxInstruct;
class SyntaxVar;
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
    static SyntaxExp* convertVar2Exp(SyntaxVar* var, ExplainContext* context);
    static SyntaxVar* convertExp2Var(SyntaxExp* exp, const string& name, ExplainContext* context);
    static SyntaxExp* convertExp2Exp(SyntaxExp* exp, const string& name, ExplainContext* context);

    bool isVar();
    SyntaxVar* getVar();

public:
    list<SyntaxInstruct*> instructs;
    SyntaxData ret;
};