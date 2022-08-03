#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxMulti.h"

class ExplainIdentifier : public ExplainBase
{
public:
    ExplainIdentifier(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("multiidentifier", "", (MyRuleExecuteFunction)&ExplainIdentifier::onExplainMultiIdentifier);
        registe("multiidentifier", "add", (MyRuleExecuteFunction)&ExplainIdentifier::onExplainMultiIdentifierAdd);

        registe("identifierpath", "", (MyRuleExecuteFunction)&ExplainIdentifier::onExplainIdentifierPath);
        registe("identifierpath", "add", (MyRuleExecuteFunction)&ExplainIdentifier::onExplainIdentifierPathAdd);
    }

    Result onExplainMultiIdentifier(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<string>* mi = new SyntaxMulti<string>(context);
        mi->items.push_back(es[0].remark);

        out.ptr = mi;
        return {};
    }

    Result onExplainMultiIdentifierAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<string>* mi = (SyntaxMulti<string>*)es[0].ptr;
        mi->items.push_back(es[2].remark);
        
        out.ptr = mi;
        return {};
    }

    Result onExplainIdentifierPath(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<string>* mi = new SyntaxMulti<string>(context);
        mi->items.push_back(es[0].remark);

        out.ptr = mi;
        return {};
    }

    Result onExplainIdentifierPathAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<string>* mi = (SyntaxMulti<string>*)es[0].ptr;
        mi->items.push_back(es[2].remark);
        
        out.ptr = mi;
        return {};
    }
};
