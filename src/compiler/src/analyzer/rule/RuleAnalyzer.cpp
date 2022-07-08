#include "RuleAnalyzer.h"
#include "StringUtils.h"
#include "../lex/LexAnalyzer.h"
using namespace OneCommon;

RuleAnalyzer::RuleAnalyzer()
{
}

RuleAnalyzer::~RuleAnalyzer()
{
}

void RuleAnalyzer::parse(const string& script, RuleBlock* block)
{
	LexAnalyzer analyzer;
	int rulePriority = 10000;

	list<string> lines = StringUtils::getLines(script);
	for (auto& line : lines)
	{
		list<MyRule> myRules = getRules(line);
		for (auto& myRule : myRules)
		{
			list<LexElement> elements = analyzer.analysis(myRule.value);
			if (elements.empty())
			{
				continue;
			}

			Rule* newRule = new Rule(0, myRule.name, block->addRuleName(myRule.name), 
				myRule.alias, block->addRuleName(myRule.alias), rulePriority);
			newRule->elements.reserve(elements.size());
			for (auto& ele : elements)
			{
				if (ele.type == Element::IDENTIFIER)
				{
					if (ele.remark == "IDENTIFIER")
					{
						newRule->elements.push_back(Element(Element::IDENTIFIER));
					}
					else if (ele.remark == "STRING")
					{
						newRule->elements.push_back(Element(Element::STRING));
					}
					else if (ele.remark == "CHAR")
					{
						newRule->elements.push_back(Element(Element::CHAR));
					}
					else if (ele.remark == "NUMBER")
					{
						newRule->elements.push_back(Element(Element::NUMBER));
					}
					else if (ele.remark == "LINEEND")
					{
						newRule->elements.push_back(Element(Element::LINEEND));
					}
					else if (ele.remark == "FILEEND")
					{
						newRule->elements.push_back(Element(Element::FILEEND));
					}
					else
					{
						newRule->elements.push_back(Element(Element::RULE, block->addRuleName(ele.remark)));
					}
				}
				else if (ele.type == Element::STRING)
				{
					newRule->elements.push_back(Element(Element::KEYWORD, block->addKeyword(ele.remark)));
				}
				else if (ele.type == Element::FILEEND)
				{
					//跳过
				}
				else
				{
					newRule->elements.push_back(Element(ele.type, ele.value));
				}
			}

			if (newRule->validate() == false)
			{
				delete newRule;
				continue;
			}

			block->addRule(newRule);
		}

		rulePriority--;
	}
}

list<RuleAnalyzer::MyRule> RuleAnalyzer::getRules(string& line)
{
	//去掉注释
	string::size_type noteIndex = line.find('#');
	if (noteIndex != line.npos)
	{
		line = line.substr(0, noteIndex);
	}

	list<MyRule> rules;
	list<string> ruleStrings = StringUtils::splitString(line, ";;");
	for (auto& ruleString : ruleStrings)
	{
		MyRule rule = parseRule(ruleString);
		if (rule.name.empty())
		{
			continue;
		}
		rules.push_back(rule);
	}
	return rules;
}
	
RuleAnalyzer::MyRule RuleAnalyzer::parseRule(const string& ruleString)
{
	MyRule rule;
	do
	{
		string::size_type sep = ruleString.find('=');
		if (sep == string::npos)
		{
			break;
		}

		string key = StringUtils::trim(ruleString.substr(0, sep));
		string value = StringUtils::trim(ruleString.substr(sep + 1));
		if (key.empty() || value.empty())
		{
			break;
		}

		//解析名字
		string::size_type left = key.find('(');
		string::size_type right = key.find_last_of(')');
		if (left != string::npos && right != string::npos && right > left)
		{
			rule.name = key.substr(0, left);
			rule.alias = key.substr(left + 1, right - left - 1);
		}
		else
		{
			rule.name = std::move(key);
		}
		rule.value = std::move(value);
	} while(0);
	return rule;
}

