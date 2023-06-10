#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxVar.h"
#include "../syntax/SyntaxType.h"
#include "../syntax/SyntaxExp.h"

class ExplainVar : public ExplainBase
{
public:
    ExplainVar(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("var", "typepath", (MyRuleExecuteFunction)&ExplainVar::onExplainVarTypePath);
        registe("var", "exp", (MyRuleExecuteFunction)&ExplainVar::onExplainVarExp);
        registe("var", "expthis", (MyRuleExecuteFunction)&ExplainVar::onExplainVarExp);
        registe("var", "expsuper", (MyRuleExecuteFunction)&ExplainVar::onExplainVarExp);
        registe("var", "expclass", (MyRuleExecuteFunction)&ExplainVar::onExplainVarExp);
        registe("var", "this", (MyRuleExecuteFunction)&ExplainVar::onExplainVarKeyword);
        registe("var", "super", (MyRuleExecuteFunction)&ExplainVar::onExplainVarKeyword);
    }

    Result onExplainVarTypePath(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxType* type = (SyntaxType*)es[0].ptr;

        SyntaxVar* var = new SyntaxVar(context);
        var->items = std::move(type->items);

        out.ptr = var;
        return {};
    }

    Result onExplainVarExp(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[0].ptr;

        SyntaxVar* var = SyntaxExp::convertExp2Var(exp, es[2].remark, context);

        out.ptr = var;
        return {};
    }

    Result onExplainVarKeyword(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVar* var = new SyntaxVar(context);

        var->addItem(es[0].remark, context);

        out.ptr = var;
        return {};
    }
};