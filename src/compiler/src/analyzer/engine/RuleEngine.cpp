#include "RuleEngine.h"

RuleEngine::RuleEngine(RuleMap& ruleMap)
	:ruleMap(ruleMap)
{
}
	
void RuleEngine::setRuleExecuteFuncion(RuleExecuteFunction func)
{
	this->cbFunc = func;
}

Result RuleEngine::execute(list<LexElement>& es)
{
	this->waitEles.clear();
	this->waitEles.reserve(es.size());
	for (auto iter = es.rbegin();iter != es.rend();++iter)
	{
		this->waitEles.push_back(std::move(*iter));
	}
	this->exeStack.clear();
	this->exeStack.reserve(es.size() / 5);

	ExeStep step;
	step.link = ruleMap.getStartLink();
	exeStack.push_back(step);

	Element root = ruleMap.ruleContainer.getRootElement();

	while (true)
	{
		VR(executeElement());

		if (waitEles.size() == 1 && exeStack.size() == 1)
		{
			LexElement& lastElement = waitEles.back();
			if (lastElement.compare(root) == 0)
			{
				return {};
			}
		}
	}

	return {};
}

Result RuleEngine::executeElement()
{
	RuleMapLink* link = exeStack.back().link;

	//去除无效元素
	while (waitEles.empty() == false)
	{
		Element& element = waitEles.back();
		if ((element.type == LexElement::NOTE)
			|| (element.type == LexElement::LINEEND))
		{
			//忽略换行和注释
			waitEles.pop_back();
		}
		else
		{
			break;
		}
	}

	//判断是否要归约
	Element element;
	if (waitEles.empty() == false)
	{
		element = waitEles.back();
	}
	RuleStep finishRuleStep = link->node->finishRuleStep;
	if (finishRuleStep.empty() == false)
	{
		auto iter = link->node->nextNodes.find(element);
		if (iter != link->node->nextNodes.end())
		{
			RuleMapLink* nextLink = iter->second;
			RuleMapNode* nextNode = nextLink->node;
			if (finishRuleStep.rule->priority < nextNode->maxPriority)
			{
				finishRuleStep.clear();
			}
		}
		if (finishRuleStep.empty() == false)
		{
			VR(finishRule(finishRuleStep.rule));
			return {};
		}
	}

	if (element.empty())
	{
		return rollback();
	}
	auto iter = link->node->nextNodes.find(element);
	if (iter == link->node->nextNodes.end())
	{
		//无法继续，回滚
		return rollback();
	}
	RuleMapLink* next = iter->second;

	ExeStep step;
	step.element = std::move(waitEles.back());
	step.link = next;
	exeStack.push_back(step);
	waitEles.pop_back();

	return {};
}
	
Result RuleEngine::rollback()
{
	while (exeStack.empty() == false)
	{
		RuleStep finishRuleStep = exeStack.back().link->node->finishRuleStep;
		if (finishRuleStep.empty() == false)
		{
			//激活规则
			VR(finishRule(finishRuleStep.rule));
			return {};
		}

		waitEles.push_back(exeStack.back().element);
		exeStack.pop_back();
	}
	return R_FAILED;
}
	
Result RuleEngine::finishRule(Rule* rule)
{
	//取出堆栈
	vector<RuleMapLink*> links;
	vector<LexElement> es;
	es.resize(rule->elements.size());
	links.resize(es.size());
	for (int i = 0;i < (int)es.size();i++)
	{
		ExeStep& exeStep = exeStack.back();
		int index = (int)es.size() - 1 - i;

		links[index] = exeStep.link;
		es[index] = std::move(exeStep.element);

		exeStack.pop_back();
	}

	//处理延迟规则
	VR(executeDelayRule(rule, links, es));

	//
	LexElement out;
	VR(executeRule(rule, es, out));

	waitEles.push_back(std::move(out));
	return {};
}
	
Result RuleEngine::executeRule(Rule* rule, vector<LexElement>& es, LexElement& out)
{
	printf("executeRule name=%s alias=%s\n", rule->name.c_str(), rule->alias.c_str());
	
	//参数校验
#ifndef NDEBUG
	for (int i = 0;i < (int)es.size();i++)
	{
		Element& ele = es[i];
		if (!(ele == rule->elements[i]))
		{
			assert(false);
		}
	}
#endif

	out.type = Element::RULE;
	out.value = rule->nameIndex;
	out.ptr = es[0].ptr;
	out.start = es.front().start;
	out.end = es.back().end;
	if (es.size() == 1)
	{
		out.remark = es[0].remark;
	}

	if (cbFunc)
	{
		return cbFunc(rule, es, out);
	}
	else
	{
		return {};
	}
}
	
Result RuleEngine::executeDelayRule(Rule* rule, vector<RuleMapLink*>& links, vector<LexElement>& es)
{
	for (int i = 0;i < rule->elements.size();i++)
	{
		RuleMapLink* link = links[i];

		RuleStep ruleStep(rule, i + 1);
		auto iter = link->explainMap.find(ruleStep);
		if (iter != link->explainMap.end())
		{
			vector<Rule*>& exeRules = iter->second;
			for (auto exeRule : exeRules)
			{
				vector<LexElement> es2;
				es2.push_back(std::move(es[i]));
				LexElement out;
				VR(executeRule(exeRule, es2, out));
				es[i] = std::move(out);
			}
		}
	}
	return {};
}
