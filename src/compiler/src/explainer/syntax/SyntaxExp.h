#pragma once
#include "SyntaxBase.h"
#include "SyntaxData.h"

class SyntaxInstruct;
class SyntaxExp : public SyntaxBase
{
public:
    SyntaxExp(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

    void append(SyntaxExp* exp, bool start = false)
    {
        if (start)
        {
            this->instructs.splice(this->instructs.begin(), exp->instructs);
        }
        else
        {
            this->instructs.splice(this->instructs.end(), exp->instructs);
        }
    }

public:
    list<SyntaxInstruct*> instructs;
    SyntaxData ret;
};