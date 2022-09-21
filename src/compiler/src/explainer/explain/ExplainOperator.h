#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxFunc.h"
#include "../syntax/SyntaxMulti.h"
#include "../syntax/SyntaxVar.h"
#include "../syntax/SyntaxType.h"

class ExplainOperator : public ExplainBase
{
public:
    ExplainOperator(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("inc", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);
        registe("dec", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);
        registe("rbitmov2", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);
        registe("rbitmov", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);
        registe("lbitmov", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);
        registe("gte", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);
        registe("lte", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);
        registe("eqd", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);
        registe("eq", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);
        registe("neq", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);
        registe("and", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);
        registe("or", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);
        registe("arr", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);
        registe("dot3", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperator);

        registe("operatordefheader", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperatorDefHeader);
        registe("operatordef", "", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperatorDef);
        registe("operatordef", "virtual", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperatorDefVirtual);
        registe("operatordef", "vnormal", (MyRuleExecuteFunction)&ExplainOperator::onExplainOperatorDefVNormal);
    }

    Result onExplainOperator(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        out.remark.clear();
        for (auto& ele : es)
        {
            out.remark += ele.remark;
        }
        return {};
    }

    Result onExplainOperatorDefHeader(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxFunc* func = new SyntaxFunc(context);
        func->name = es[2].remark;
        func->returnType = (SyntaxType*)es[0].ptr;
        func->params = std::move(((SyntaxMulti<SyntaxVarDef*>*)es[3].ptr)->items);

        out.ptr = func;
        return {};
    }
    
    Result onExplainOperatorDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxFunc* func = (SyntaxFunc*)es[0].ptr;
        SyntaxBlock* block = (SyntaxBlock*)es[1].ptr;

        func->block = block;

        out.ptr = func;
        return {};
    }
    
    Result onExplainOperatorDefVirtual(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxFunc* func = (SyntaxFunc*)es[1].ptr;
        func->isVirtual = true;
        out.ptr = func;
        return {};
    }
    
    Result onExplainOperatorDefVNormal(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxFunc* func = (SyntaxFunc*)es[1].ptr;
        SyntaxBlock* block = (SyntaxBlock*)es[2].ptr;

        func->isVirtual = true;
        func->block = block;
        out.ptr = func;
        return {};
    }
};