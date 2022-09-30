#include "SyntaxExp.h"
#include "SyntaxInstruct.h"

void SyntaxExp::append(SyntaxExp* exp, bool start)
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

SyntaxExp* SyntaxExp::combine(SyntaxMulti<SyntaxExp*>* multiExp, bool addCommaInstruct, ExplainContext* context)
{
    if (multiExp->items.size() > 1)
    {
        SyntaxExp* exp = new SyntaxExp(context);
        if (addCommaInstruct)
        {
            SyntaxInstruct* instruct  = new SyntaxInstruct(context);
            instruct->cmd = COMMA;
            for (auto& item : multiExp->items)
            {
                instruct->params.push_back(item->ret);
                exp->append(item);
            }
            exp->instructs.push_back(instruct);
            exp->ret.setInstruct(instruct);
        }
        else
        {
            for (auto& item : multiExp->items)
            {
                exp->append(item);
            }
        }
        return exp;
    }
    else
    {
        return multiExp->items.front();
    }
}
