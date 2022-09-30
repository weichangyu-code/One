#pragma once
#include "../map/RuleMap.h"
#include "../../common/Result.h"
#include "../lex/LexElement.h"
#include <functional>

typedef function<Result(Rule*, vector<LexElement>&, LexElement&)> RuleExecuteFunction;

class RuleEngine
{
public:
	RuleEngine(RuleMap& ruleMap);

public:
	Result execute(list<LexElement>& es);
	void setRuleExecuteFuncion(RuleExecuteFunction func);

protected:
	Result executeElement();

	Result rollback();
	Result finishRule(Rule* rule);
	Result executeRule(Rule* rule, vector<LexElement>& es, LexElement& out);
	Result executeDelayRule(Rule* rule, vector<RuleMapLink*>& links, vector<LexElement>& es);

protected:
	struct ExeStep
	{
		RuleMapLink*	link = nullptr;
		LexElement		element;
	};

	RuleMap& ruleMap;

	vector<LexElement>	waitEles;
	vector<ExeStep>		exeStack;

	RuleExecuteFunction cbFunc;
};
