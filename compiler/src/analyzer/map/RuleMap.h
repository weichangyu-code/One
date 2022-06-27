#pragma once
#include "RuleMapNode.h"
#include "RuleContainer.h"
#include "RuleMapLink.h"

class RuleMap
{
public:
	RuleMap();
	~RuleMap();

public:
	bool init(const string& script);

	RuleMapLink* getStartLink() {return links.front();}

	const vector<RuleMapLink*>& getLinks() const {return links;}
	const vector<RuleMapNode*>& getNodes() const {return nodes;}

protected:
	void profectNode(RuleMapNode* node, list<RuleMapNode*>& unProfectNodes);
	RuleMapLink* createLink(const RuleMapNodeKey& key);
	RuleMapNode* getAndAddNode(const RuleMapNodeKey& key);
	RuleMapLink* explainNode(RuleMapNode* parent, const Element& element, RuleMapLink* link, set<Element>& explained);

public:
	RuleBlock block;
	RuleContainer ruleContainer;
protected:
	vector<RuleMapLink*> links;
	vector<RuleMapNode*> nodes;

	//临时变量
	unordered_map<RuleMapNodeKey, RuleMapNode*> nodeMap;
	list<RuleMapNode*> unProfectNodes;
};

