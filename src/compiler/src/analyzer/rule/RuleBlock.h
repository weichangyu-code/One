#pragma once
#include "Common.h"
#include "Rule.h"

class RuleBlock
{
public:
    RuleBlock()
    {
    }
    ~RuleBlock()
    {
        for (auto& rule : rules)
        {
            delete rule;
        }
    }

    void addKeywords(const char* keywords[], int size)
    {
        this->keywords.resize(size);
        for (int i = 0;i < size;i++)
        {
            this->keywords[i] = keywords[i];
            this->keywordMap.insert(make_pair(keywords[i], i));
        }
    }

    int addKeyword(const string& keyword)
    {
        auto iter = keywordMap.find(keyword);
        if (iter == keywordMap.end())
        {
            int index = keywords.size();
            keywordMap.insert(make_pair(keyword, index));
            keywords.push_back(keyword);
            return index;
        }
        return iter->second;
    }

    int getKeywordIndex(const string& keyword)
    {
        auto iter = keywordMap.find(keyword);
        if (iter == keywordMap.end())
        {
            return -1;
        }
        return iter->second;
    }

    int addRuleName(const string& ruleName)
    {
        auto iter = ruleNameMap.find(ruleName);
        if (iter == ruleNameMap.end())
        {
            int index = ruleNames.size();
            ruleNameMap.insert(make_pair(ruleName, index));
            ruleNames.push_back(ruleName);
            return index;
        }
        return iter->second;
    }

    int getRuleNameIndex(const string& ruleName)
    {
        auto iter = ruleNameMap.find(ruleName);
        if (iter == ruleNameMap.end())
        {
            return -1;
        }
        return iter->second;
    }

    void addRuleNames(const char* ruleNames[], int size)
    {
        this->ruleNames.resize(size);
        for (int i = 0;i < size;i++)
        {
            this->ruleNames[i] = ruleNames[i];
            this->ruleNameMap.insert(make_pair(ruleNames[i], i));
        }
    }

    void addRule(Rule* rule)
    {
        rule->index = rules.size();
        rules.push_back(rule);
    }

public:
    vector<string> keywords;    //关键字,从小到大
    vector<string> ruleNames;
    vector<Rule*>  rules;

protected:
    map<string, int> keywordMap;
    map<string, int> ruleNameMap;
};
