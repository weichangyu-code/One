#pragma once
#include "RuleStep.h"
#include "RuleMapNode.h"

class RuleMapLink
{
public:
    RuleMapLink() = default;
    RuleMapLink(int index)
    {
        this->index = index;
    }
    RuleMapLink(int index, RuleMapNode* node)
    {
        this->index = index;
        this->node = node;
        for (auto& step : node->ruleSteps)
        {
            explainMap[step] = {};
        }
    }

    void clearEmptyExplain()
    {
        for (auto iter = explainMap.begin();iter != explainMap.end();)
        {
            auto now = iter++;
            if (now->second.empty())
            {
                explainMap.erase(now);
            }
        }
    }

public:
    int index = 0;
    map<RuleStep, vector<Rule*>> explainMap;
    RuleMapNode* node = nullptr;
};