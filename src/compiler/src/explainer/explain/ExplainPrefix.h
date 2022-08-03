#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxMulti.h"

class ExplainPrefix : public ExplainBase
{
public:
    ExplainPrefix(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
		registe("vardefprefix", "", (MyRuleExecuteFunction)&ExplainPrefix::onExeFuncPrefix);
		registe("vardefprefix", "add", (MyRuleExecuteFunction)&ExplainPrefix::onExeFuncPrefixAdd);
    }

	Result onExeFuncPrefix(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxMulti<string>* prefix = new SyntaxMulti<string>(context);
		prefix->items.push_back(es[0].remark);
        out.ptr = prefix;
		return {};
	}
	Result onExeFuncPrefixAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxMulti<string>* prefix = (SyntaxMulti<string>*)es[0].ptr;
		prefix->items.push_back(es[1].remark);
        out.ptr = prefix;
		return {};
	}
};

