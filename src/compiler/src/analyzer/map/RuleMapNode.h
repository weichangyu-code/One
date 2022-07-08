#pragma once
#include "RuleStep.h"
#include "CollectionUtils.h"
using namespace OneCommon;

class RuleMapNodeKey
{
public:
	int compare(const RuleMapNodeKey& r) const
	{
		return CollectionUtils::compare(this->ruleSteps, r.ruleSteps, 
			[](set<RuleStep>::const_reference v1, set<RuleStep>::const_reference v2)
			{
				return v1.compare(v2);
			});
	}

	bool operator < (const RuleMapNodeKey& r) const
	{
		return compare(r) < 0;
	}

	bool operator == (const RuleMapNodeKey& r) const
	{
		return compare(r) == 0;
	}

	size_t hash() const
	{
		size_t h = 0;
		for (auto& step : ruleSteps)
		{
			h += step.uv;
		}
		return h;
	}

public:
	set<RuleStep> ruleSteps;
};

namespace std
{
    template<> struct hash<RuleMapNodeKey>
    {
        std::size_t operator()(RuleMapNodeKey const& key) const noexcept
        {
            return key.hash();
        }
    };
}

class RuleMapLink;
class RuleMapNode
{
public:
	RuleMapNode() = default;
	RuleMapNode(int index)
	{
		this->index = index;
	}
	~RuleMapNode()
	{
	}

public:
	int index = 0;
	set<RuleStep> ruleSteps;
	set<RuleStep> expandRuleSteps;
	map<Element, RuleMapLink*> nextNodes;		//CRuleStep*，保存上个节点规则，表示由哪个规则衍生的，便以提前规约

	//辅助参数
	RuleStep finishRuleStep;
	int maxPriority = 0;
};

