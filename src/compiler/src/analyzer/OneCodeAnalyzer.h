#pragma once
#include "one/OneRuleMap.h"
#include "engine/RuleEngine.h"
#include "engine/CodeAnalyzer.h"

class OneCodeAnalyzer
{
public:
	OneCodeAnalyzer();
	~OneCodeAnalyzer();

public:
	Result analysis(const string& code);
	void setRuleExecuteFuncion(RuleExecuteFunction func);

public:
	OneRuleMap ruleMap;
	RuleEngine ruleEngine;
	CodeAnalyzer codeAnalyzer;
};
