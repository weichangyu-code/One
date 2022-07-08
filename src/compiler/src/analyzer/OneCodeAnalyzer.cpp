#include "OneCodeAnalyzer.h"

OneCodeAnalyzer::OneCodeAnalyzer()
	: ruleEngine(ruleMap), codeAnalyzer(&ruleMap.block)
{
	ruleMap.initOne();
}


OneCodeAnalyzer::~OneCodeAnalyzer()
{
}

Result OneCodeAnalyzer::analysis(const string& code)
{
	list<LexElement> elements = codeAnalyzer.analysis(code);
	return ruleEngine.execute(elements);
}
	
void OneCodeAnalyzer::setRuleExecuteFuncion(RuleExecuteFunction func)
{
	ruleEngine.setRuleExecuteFuncion(func);
}
