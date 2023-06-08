#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxType.h"
#include "../syntax/SyntaxMulti.h"

class ExplainType : public ExplainBase
{
public:
    ExplainType(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        
        registe("calltemplate", "", (MyRuleExecuteFunction)&ExplainType::onExplainCallTemplate);
        registe("calltemplate", "null", (MyRuleExecuteFunction)&ExplainType::onExplainCallTemplateNull);

        registe("typepathitem", "template", (MyRuleExecuteFunction)&ExplainType::onExplainTypePathItemTemplate);
        registe("typepathitem", "identifier", (MyRuleExecuteFunction)&ExplainType::onExplainTypePathItem);
        registe("typepath", "", (MyRuleExecuteFunction)&ExplainType::onExplainTypePath);
        registe("typepath", "add", (MyRuleExecuteFunction)&ExplainType::onExplainTypePathAdd);
        registe("type", "arr", (MyRuleExecuteFunction)&ExplainType::onExplainTypeArr);
        registe("type", "func", (MyRuleExecuteFunction)&ExplainType::onExplainTypeFunc);
        registe("type", "funcparam", (MyRuleExecuteFunction)&ExplainType::onExplainTypeFuncParam);
        registe("multitype", "", (MyRuleExecuteFunction)&ExplainType::onExplainMultiType);
        registe("multitype", "add", (MyRuleExecuteFunction)&ExplainType::onExplainMultiTypeAdd);
    }

    Result onExplainCallTemplate(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxType*>* syntax = (SyntaxMulti<SyntaxType*>*)es[1].ptr;
        out.ptr = syntax;
        return {};
    }

    Result onExplainCallTemplateNull(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxType*>* syntax = new SyntaxMulti<SyntaxType*>(context);
        out.ptr = syntax;
        return {};
    }

    Result onExplainTypePathItemTemplate(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxType*>* multiType = (SyntaxMulti<SyntaxType*>*)es[1].ptr;

        SyntaxTypePathItem* syntax = new SyntaxTypePathItem(context);
        syntax->typeName = es[0].remark;
        syntax->templateTypes = std::move(multiType->items);

        out.ptr = syntax;
        return {};
    }

    Result onExplainTypePathItem(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxTypePathItem* syntax = new SyntaxTypePathItem(context);
        syntax->typeName = es[0].remark;
        out.ptr = syntax;
        return {};
    }

    Result onExplainTypePath(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxType* syntax = new SyntaxType(context);
        syntax->items.push_back((SyntaxTypePathItem*)es[0].ptr);
        out.ptr = syntax;
        return {};
    }

    Result onExplainTypePathAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxType* syntax = (SyntaxType*)es[0].ptr;
        syntax->items.push_back((SyntaxTypePathItem*)es[2].ptr);
        out.ptr = syntax;
        return {};
    }

    Result onExplainTypeArr(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxType* syntax = (SyntaxType*)es[0].ptr;
        syntax->arrNum++;
        out.ptr = syntax;
        return {};
    }

    Result onExplainTypeFunc(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxType* syntax = (SyntaxType*)es[0].ptr;
        syntax->funcType = true;
        out.ptr = syntax;
        return {};
    }

    Result onExplainTypeFuncParam(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxType* syntax = (SyntaxType*)es[0].ptr;
        SyntaxMulti<SyntaxType*>* multiType = (SyntaxMulti<SyntaxType*>*)es[2].ptr;

        syntax->funcType = true;
        syntax->funcParamTypes = std::move(multiType->items);
        out.ptr = syntax;
        return {};
    }

    Result onExplainMultiType(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxType*>* syntax = new SyntaxMulti<SyntaxType*>(context);
        syntax->items.push_back((SyntaxType*)es[0].ptr);
        out.ptr = syntax;
        return {};
    }
    Result onExplainMultiTypeAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxType*>* syntax = (SyntaxMulti<SyntaxType*>*)es[0].ptr;
        syntax->items.push_back((SyntaxType*)es[2].ptr);
        out.ptr = syntax;
        return {};
    }
};




