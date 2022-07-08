#pragma once
#include "Common.h"
#include "analyzer/map/RuleContainer.h"
#include "analyzer/engine/RuleEngine.h"
#include "analyzer/rule/RuleBlock.h"

class ExplainBase;
class RuleContainer;
class ExplainContext;
class ExplainContainer
{
public:
    ExplainContainer(RuleContainer* ruleContainer, ExplainContext* context);
    ~ExplainContainer();

public:
	void setRuleExecuteFuncion(const string& ruleName, const string& alias, RuleExecuteFunction func);

    Result callRuleExecuteFunction(Rule* rule, vector<LexElement>& es, LexElement& out);

protected:
    RuleContainer* ruleContainer;
    vector<RuleExecuteFunction> funcs;
    list<ExplainBase*> explains;
};

