#pragma once
#include "../rule/Rule.h"

class RuleBlock;
class RuleContainer
{
public:
	RuleContainer();
	~RuleContainer();

public:
	void init(RuleBlock* block);

	Element 				getRootElement() {return {Element::RULE, rootRule};}
	const list<Rule*>& 		getRootRules() const {return getRules(rootRule);}
	const list<Rule*>&		getRules(int nameIndex) const;
	const list<Rule*>&		getRules(const string& name) const;

public:
	RuleBlock* block = nullptr;
protected:
	int rootRule = 0;
	vector<list<Rule*>> rules;
};
