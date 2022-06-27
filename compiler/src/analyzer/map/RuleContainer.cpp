#include "RuleContainer.h"
#include "../rule/RuleBlock.h"

RuleContainer::RuleContainer()
{
}

RuleContainer::~RuleContainer()
{
}

void RuleContainer::init(RuleBlock* block)
{
	this->block = block;

	//保存到内存
	rules.resize(block->ruleNames.size());
	for (auto& rule : block->rules)
	{
		if (rule->elements.back().type == Element::FILEEND)
		{
			rootRule = rule->nameIndex;
		}
		rules[rule->nameIndex].push_back(rule);
	}
}
	
const list<Rule*>& RuleContainer::getRules(int nameIndex) const
{
	return rules[nameIndex];
}
	
const list<Rule*>& RuleContainer::getRules(const string& name) const
{
	int index = block->getRuleNameIndex(name);
	return getRules(index);
}
	
// list<Rule*> RuleContainer::explainRule(Rule* rule)
// {
// 	list<Rule*> ruleExes;
// 	Rule* newRuleEx = new Rule(*rule);
// 	newRuleEx->explainMap.reserve(newRuleEx->elements.size());
// 	ruleExes.push_back(newRuleEx);

// 	vector<list<ElementExplain>> elementExplainses;
// 	elementExplainses.reserve(rule->elements.size());
// 	int expend = 1;
// 	if (rule->elements.size() == 1)
// 	{
// 		elementExplainses.push_back({});
// 	}
// 	else
// 	{
// 		for (auto& ele : rule->elements)
// 		{
// 			list<ElementExplain> explainMap = relationElementMap.getExplains(ele);
// 			expend *= explainMap.empty() ? 1 : explainMap.size();
// 			elementExplainses.push_back(std::move(explainMap));
// 		}
// 	}

// 	for (int i = 0;i < elementExplainses.size();i++)
// 	{
// 		list<ElementExplain>& explainMap = elementExplainses[i];
// 		if (explainMap.empty())
// 		{
// 			for (auto& newRuleEx : ruleExes)
// 			{
// 				newRuleEx->explainMap.push_back({newRuleEx->elements[i], {}});
// 			}
// 		}
// 		else if (explainMap.size() == 1)
// 		{
// 			ElementExplain& explain = explainMap.front();
// 			for (auto& newRuleEx : ruleExes)
// 			{
// 				newRuleEx->explainMap.push_back(explain);
// 			}
// 		}
// 		else
// 		{
// 			//先添加新建的
// 			list<Rule*> tmp = ruleExes;
// 			auto iter = explainMap.begin();
// 			for (++iter;iter != explainMap.end();++iter)
// 			{
// 				ElementExplain& explain = *iter;
// 				for (auto& newRuleEx : ruleExes)
// 				{
// 					Rule* newRuleEx2 = new Rule(*newRuleEx);
// 					newRuleEx2->explainMap.push_back(explain);
// 					tmp.push_back(newRuleEx2);
// 				}
// 			}

// 			ElementExplain& explain = explainMap.front();
// 			for (auto& newRuleEx : ruleExes)
// 			{
// 				newRuleEx->explainMap.push_back(explain);
// 			}

// 			swap(tmp, ruleExes);
// 		}
// 	}

// 	printf("rule=%s expend=%d\n", rule->name.c_str(), ruleExes.size());
// 	if (rule->name == "import")
// 	{
// 		printf("");
// 	}

// 	return ruleExes;
// }

// void RuleContainer::setRuleExecuteListener(string name, string alias, CRuleExecuteFunction func)
// {
// 	list<CRule*>& rules = allRules[name];
// 	for (list<CRule*>::iterator iter = rules.begin();iter != rules.end();++iter)
// 	{
// 		CRule* rule = *iter;
// 		if (alias.empty() || alias == rule->alias)
// 		{
// 			rule->exeFunc = func;
// 		}
// 	}
// }

// list<CRule*>& RuleContainer::getRoots()
// {
// 	if (root.empty())
// 	{
// 		static list<CRule*> empty;
// 		return empty;
// 	}
// 	return allRules[root];
// }
