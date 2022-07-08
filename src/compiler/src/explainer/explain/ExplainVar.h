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
        registe("var", "identifier", (MyRuleExecuteFunction)&onExplainVarIdentifier);
        registe("var", "typepath", (MyRuleExecuteFunction)&onExplainVarTypePath);
        registe("var", "exp", (MyRuleExecuteFunction)&onExplainVarExp);
        registe("var", "expthis", (MyRuleExecuteFunction)&onExplainVarExp);
        registe("var", "expsuper", (MyRuleExecuteFunction)&onExplainVarExp);
        registe("var", "this", (MyRuleExecuteFunction)&onExplainVarThis);
        registe("var", "super", (MyRuleExecuteFunction)&onExplainVarSuper);
    }

    Result onExplainVarIdentifier(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVar* var = new SyntaxVar(context);

        var->items.push_back(new SyntaxTypePathItem(es[0].remark, context));

        out.ptr = var;
        return {};
    }

    Result onExplainVarTypePath(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVar* var = new SyntaxVar(context);
        SyntaxType* type = (SyntaxType*)es[0].ptr;

        if (type->items.back()->templateTypes.empty() == false)
        {
            //转换成变量的话，最后一个不能是模板
            return R_FAILED;
        }

        var->items = std::move(type->items);

        out.ptr = var;
        return {};
    }

    Result onExplainVarExp(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVar* var = new SyntaxVar(context);
        SyntaxExp* exp = (SyntaxExp*)es[0].ptr;

        var->exp = exp;
        var->items.push_back(new SyntaxTypePathItem(es[2].remark, context));

        out.ptr = var;
        return {};
    }

    Result onExplainVarThis(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVar* var = new SyntaxVar(context);

        var->items.push_back(new SyntaxTypePathItem(es[0].remark, context));

        out.ptr = var;
        return {};
    }

    Result onExplainVarSuper(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVar* var = new SyntaxVar(context);

        var->items.push_back(new SyntaxTypePathItem(es[0].remark, context));

        out.ptr = var;
        return {};
    }

};