#pragma once
#include "../rule/RuleBlock.h"

class RuleStep
{
public:
	RuleStep() = default;
	RuleStep(Rule* rule, int step)
	{
		this->rule = rule;
		this->step = step;
		this->ruleIndex = rule->index;
	}

	void clear()
	{
		this->rule = nullptr;
		this->step = 0;
		this->ruleIndex = 0;
	}

	RuleStep nextStep() const
	{
		RuleStep other(this->rule, this->step + 1);
		return other;
	}

	bool empty() const
	{
		return rule == nullptr;
	}

	bool finished() const
	{
		return step >= rule->elements.size();
	}

	const Element& getNextElement() const
	{
		return rule->elements[step];
	}
	
	const Element& getPrevElement() const
	{
		return rule->elements[step - 1];
	}

	int getPriority() const
	{
		return rule->priority;
	}

public:
	int compare(const RuleStep& r) const
	{
		return this->uv - r.uv;
	}

	bool operator < (const RuleStep& r) const
	{
		return compare(r) < 0;
	}

	bool operator == (const RuleStep& r) const
	{
		return compare(r) == 0;
	}
	
public:
	union
	{
		int uv = 0;
		struct
		{
			short ruleIndex;
			short step;
		};
	};
	Rule* rule = nullptr;
};
