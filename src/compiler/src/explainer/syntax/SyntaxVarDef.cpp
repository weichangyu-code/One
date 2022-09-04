#include "SyntaxVarDef.h"
#include "SyntaxExp.h"
#include "SyntaxInstruct.h"

void SyntaxVarDef::addVarDefAndAssginInstruct(ExplainContext* context)
{
    SyntaxData right;
    if (exp == nullptr)
    {
        exp = new SyntaxExp(context);
    }
    else
    {
        right = exp->ret;
    }

    SyntaxInstruct* instructLeft = new SyntaxInstruct(context);
    instructLeft->cmd = VARDEF;
    instructLeft->varDef = this;
    exp->instructs.push_back(instructLeft);
    exp->ret.setInstruct(instructLeft);
    
    if (right.isNone() == false)
    {
        SyntaxInstruct* instructAssgin = new SyntaxInstruct(context);
        instructAssgin->cmd = ASSIGN;
        instructAssgin->params.push_back(instructLeft);
        instructAssgin->params.push_back(right);
        exp->instructs.push_back(instructAssgin);
        exp->ret.setInstruct(instructAssgin);
    }
}

void SyntaxVarDef::addAssginInstruct(ExplainContext* context)
{
    if (exp == nullptr)
    {
        return;    
    }
    
    SyntaxInstruct* instructAssgin = new SyntaxInstruct(context);
    instructAssgin->cmd = ASSIGN;
    instructAssgin->params.push_back(this);
    instructAssgin->params.push_back(exp->ret);
    exp->instructs.push_back(instructAssgin);
    exp->ret.setInstruct(instructAssgin);
}
