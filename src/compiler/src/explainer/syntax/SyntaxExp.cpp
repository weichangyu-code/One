#include "SyntaxExp.h"
#include "SyntaxInstruct.h"
#include "SyntaxVar.h"
#include "SyntaxType.h"

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
    
void SyntaxExp::removeInstruct(SyntaxInstruct* instruct)
{
    auto iter = std::find(instructs.begin(), instructs.end(), instruct);
    assert(iter != instructs.end());
    instructs.erase(iter);
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
    else if (multiExp->items.size() == 1)
    {
        return multiExp->items.front();
    }
    else
    {
        return new SyntaxExp(context);
    }
}

SyntaxExp* SyntaxExp::convertVar2Exp(SyntaxVar* var, ExplainContext* context)
{
    SyntaxExp* exp = new SyntaxExp(context);
    exp->ret.setVar(var);
    return exp;
}

SyntaxVar* SyntaxExp::convertExp2Var(SyntaxExp* exp, const string& name, ExplainContext* context)
{
    if (exp->isVar())
    {
        exp->ret.var->addItem(name, context);
        return exp->ret.var;
    }
    else
    {
        return new SyntaxVar(exp, name, context);
    }
}

SyntaxExp* SyntaxExp::convertExp2Exp(SyntaxExp* exp, const string& name, ExplainContext* context)
{
    return convertVar2Exp(convertExp2Var(exp, name, context), context);
}
    
bool SyntaxExp::isVar()
{
    return ret.type == SyntaxData::VAR && instructs.empty();
}
    
SyntaxVar* SyntaxExp::getVar()
{
    if (isVar())
    {
        return ret.var;
    }
    else
    {
        return nullptr;
    }
}
