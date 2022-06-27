#include "RuleMap.h"
#include "RuleContainer.h"
#include "../rule/RuleAnalyzer.h"

RuleMap::RuleMap()
{
	
}

RuleMap::~RuleMap()
{
	for (auto& node : nodes)
	{
		delete node;
	}
	for (auto& link : links)
	{
		delete link;
	}
}

bool RuleMap::init(const string& script)
{
    RuleAnalyzer ruleAnalyzer;
    ruleAnalyzer.parse(script, &block);

	ruleContainer.init(&block);

	const list<Rule*>& rootRules = ruleContainer.getRootRules();
	if (rootRules.empty())
	{
		return false;
	}

	//添加第一个节点
	RuleMapNodeKey startNodeKey;
	for (auto rule : rootRules)
	{
		startNodeKey.ruleSteps.insert(RuleStep(rule, 0));
	}
	createLink(startNodeKey);

	//展开所有节点
	while (unProfectNodes.empty() == false)
	{
		RuleMapNode* node = unProfectNodes.front();
		unProfectNodes.pop_front();
		profectNode(node, unProfectNodes);
	}

	//清空不必要的解释
	for (auto& link : links)
	{
		link->clearEmptyExplain();
	}

	return true;
}

void RuleMap::profectNode(RuleMapNode* node, list<RuleMapNode*>& unProfectNodes)
{
	//展开规则
	list<Element> waitExtend;
	set<Element> extendedEles;
	for (auto& step : node->ruleSteps)
	{
		if (step.finished())
		{
			continue;
		}
		Element ele = step.getNextElement();
		if (ele.type == Element::RULE)
		{
			waitExtend.push_back(ele);
		}
	}
	while (waitExtend.empty() == false)
	{
		Element eleWait = waitExtend.front();
		waitExtend.pop_front();
		if (extendedEles.count(eleWait) > 0)
		{
			continue;
		}
		extendedEles.insert(eleWait);
		
		const list<Rule*>& ruleExes = ruleContainer.getRules(eleWait.value);
		for (auto rule : ruleExes)
		{
			RuleStep step(rule, 0);
			Element ele = step.getNextElement();

			//保护扩展的部分
			node->expandRuleSteps.insert(step);

			if (ele.type == Element::RULE)
			{
				waitExtend.push_back(ele);
			}
		}
	}

	//扩展下一个节点
	map<Element, RuleMapNodeKey> nextNodeKeys;
	for (auto& step : node->ruleSteps)
	{
		if (step.finished())
		{
			continue;
		}
		Element ele = step.getNextElement();

		RuleMapNodeKey& key = nextNodeKeys[ele];
		key.ruleSteps.insert(step.nextStep());
	}
	for (auto& step : node->expandRuleSteps)
	{
		Element ele = step.getNextElement();

		RuleMapNodeKey& key = nextNodeKeys[ele];
		key.ruleSteps.insert(step.nextStep());
	}

	//创建下一个节点
	for (auto& kv : nextNodeKeys)
	{
		const Element& ele = kv.first;
		const RuleMapNodeKey& key = kv.second;

		RuleMapLink* link = createLink(key);
		node->nextNodes[ele] = link;
	}

	//对于下一个节点是否展开
	set<Element> explained;
	for (auto& kv : node->nextNodes)
	{
		explainNode(node, kv.first, kv.second, explained);
	}

	//计算几个有用的参数
	for (auto& step : node->ruleSteps)
	{
		if (step.finished())
		{
			if (node->finishRuleStep.empty()
				|| node->finishRuleStep.getPriority() < step.getPriority())
			{
				node->finishRuleStep = step;
			}
		}
		node->maxPriority = max(node->maxPriority, step.getPriority());
	}
}
	
RuleMapLink* RuleMap::explainNode(RuleMapNode* parent, const Element& element, RuleMapLink* link, set<Element>& explained)
{
	//去重
	if (explained.count(element) > 0)
	{
		return link;
	}

	RuleMapNodeKey newNodeKey;
	map<RuleStep, vector<Rule*>> explainMap;

	//先查找有没有一个元素的规则，并且已经完成匹配，并且等于element
	list<RuleStep> matchSteps;
	for (auto& step : link->node->ruleSteps)
	{
		if (step.rule->elements.size() == 1
			&& step.rule->elements.front() == element
			&& step.finished())
		{
			matchSteps.push_back(step);
		}
		else
		{
			newNodeKey.ruleSteps.insert(step);
		}
	}

	//准备新节点KEY
	for (auto& step : matchSteps)
	{
		Element upElement(Element::RULE, step.rule->nameIndex);
		auto iter = parent->nextNodes.find(upElement);
		if (iter == parent->nextNodes.end())
		{
			continue;
		}
		RuleMapLink* other = iter->second;

		//确保需要节点先解释完成
		other = explainNode(parent, upElement, other, explained);

		//
		for (auto& kv : other->explainMap)
		{
			newNodeKey.ruleSteps.insert(kv.first);

			auto& rules = explainMap[kv.first];
			rules = kv.second;
			rules.insert(rules.begin(), step.rule);
		}
	}

	if (explainMap.empty())
	{
		return link;
	}

	//新建节点，替换老的
	RuleMapLink* newLink = createLink(newNodeKey);
	for (auto& kv : explainMap)
	{
		newLink->explainMap[kv.first] = std::move(kv.second);
	}
	parent->nextNodes[element] = newLink;

	//
	explained.insert(element);

	return newLink;
}

RuleMapNode* RuleMap::getAndAddNode(const RuleMapNodeKey& key)
{
	auto iter = nodeMap.find(key);
	if (iter != nodeMap.end())
	{
		return iter->second;
	}
	RuleMapNode* node = new RuleMapNode((int)nodes.size());
	node->ruleSteps = key.ruleSteps;
	nodeMap[key] = node;
	nodes.push_back(node);

	unProfectNodes.push_back(node);

	return node;
}

RuleMapLink* RuleMap::createLink(const RuleMapNodeKey& key)
{
	RuleMapLink* link = new RuleMapLink((int)links.size(), getAndAddNode(key));
	links.push_back(link);
	return link;
}

