#pragma once
#include "Common.h"
#include "Rule.h"
#include "RuleBlock.h"

/*
规则语言:
关键字：NUMBER STRING CHAR IDENTIFIER LINEEND FILEEND
注释：#号开头
规则：规则名1=规则2;;规则名1=规则2。通过;;分开同行的规则。例如：AAA=BBB * CCCC ;; AAA=BBB / CCC
优先级：行号小优先级高，同行优先级相等
顶级规则：FILEEND所在的规则为顶级规则
*/

class RuleAnalyzer
{
public:
	RuleAnalyzer();
	~RuleAnalyzer();

public:
	void parse(const string& script, RuleBlock* block);

protected:
	struct MyRule
	{
		string name;
		string alias;
		string value;
	};
	list<MyRule> getRules(string& line);
	MyRule parseRule(const string& ruleString);
};

