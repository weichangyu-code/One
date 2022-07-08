#include "OneRuleMap.h"
#include "OneRuleMap.inl"
#include "StringUtils.h"
#include "SystemUtils.h"

void OneRuleMap::saveStrings(OneOStream& stream)
{
    stream.writeKeywords(block.keywords);
    stream.writeRuleNames(block.ruleNames);
}
    
void OneRuleMap::loadStrings(OneIStream& stream)
{
    int size = 0;
    while (oneKeywords[size] != nullptr)
    {
        size++;
    }
    block.addKeywords(oneKeywords, size);

    size = 0;
    while (oneRuleNames[size] != nullptr)
    {
        size++;
    }
    block.addRuleNames(oneRuleNames, size);
}
    
void OneRuleMap::saveRules(OneOStream& stream)
{
    stream << block.rules.size();
    for (auto& rule : block.rules)
    {
        stream << rule->nameIndex;
        stream << rule->aliasIndex;
        stream << rule->priority;
        stream << rule->elements.size();
        for (auto& element : rule->elements)
        {
            stream << element.uv;
        }
    }
}
    
void OneRuleMap::loadRules(OneIStream& stream)
{
    int ii;
    stream >> ii;
    for (int i = 0;i < ii;i++)
    {
        Rule* rule = new Rule();
        stream >> rule->nameIndex;
        rule->name = block.ruleNames[rule->nameIndex];
        stream >> rule->aliasIndex;
        rule->alias = block.ruleNames[rule->aliasIndex];
        stream >> rule->priority;
        int jj;
        stream >> jj;
        rule->elements.resize(jj);
        for (int j = 0;j < jj;j++)
        {
            stream >> rule->elements[j].uv;
        }
        block.addRule(rule);
    }
}
    
void OneRuleMap::saveNodes(OneOStream& stream)
{
    stream << nodes.size();
    for(auto& node : nodes)
    {
        stream << node->finishRuleStep.uv;
        stream << node->maxPriority;
        stream << node->ruleSteps.size();
        for (auto& step : node->ruleSteps)
        {
            stream << step.uv;
        }
    }
}

void OneRuleMap::loadNodes(OneIStream& stream)
{
    int ii;
    stream >> ii;
    nodes.reserve(ii);
    for (int i = 0;i < ii;i++)
    {
        RuleMapNode* node = new RuleMapNode(i);
        stream >> node->finishRuleStep.uv;
        if (node->finishRuleStep.step > 0)
        {
            //完成的规则，STEP肯定大于0
            node->finishRuleStep.rule = block.rules[node->finishRuleStep.ruleIndex];
        }
        stream >> node->maxPriority;
        int jj;
        stream >> jj;
        for (int j = 0;j < jj;j++)
        {
            RuleStep step;
            stream >> step.uv;
            step.rule = block.rules[step.ruleIndex];
            node->ruleSteps.insert(step);
        }
        nodes.push_back(node);
    }
}
    
void OneRuleMap::saveLinks(OneOStream& stream)
{
    stream << links.size();
    for (auto& link : links)
    {
        stream << link->node->index;
        stream << link->explainMap.size();
        for (auto& kv : link->explainMap)
        {
            const RuleStep& step = kv.first;
            const vector<Rule*>& rules = kv.second;

            stream << step.uv;
            stream << rules.size();
            for (auto& rule : rules)
            {
                stream << rule->index;
            }
        }
    }
}
    
void OneRuleMap::loadLinks(OneIStream& stream)
{
    int ii;
    stream >> ii;
    links.reserve(ii);
    for (int i = 0;i < ii;i++)
    {
        RuleMapLink* link = new RuleMapLink(i);
        int index;
        stream >> index;
        link->node = nodes[index];
        int jj;
        stream >> jj;
        for (int j = 0;j < jj;j++)
        {
            RuleStep step;
            stream >> step.uv;
            step.rule = block.rules[step.ruleIndex];

            int nn;
            stream >> nn;
            if (nn > 0)
            {
                vector<Rule*>& explainRules = link->explainMap[step];
                explainRules.reserve(nn);
                for (int n = 0;n < nn;n++)
                {
                    stream >> index;
                    explainRules.push_back(block.rules[index]);
                }
            }
        }
        links.push_back(link);
    }
}
    
void OneRuleMap::saveNodeLinks(OneOStream& stream)
{
    for (auto& node : nodes)
    {
        stream << node->nextNodes.size();
        for (auto& kv : node->nextNodes)
        {
            const Element& element = kv.first;
            RuleMapLink* link = kv.second;

            stream << element.uv;
            stream << link->index;
        }
    }
}
    
void OneRuleMap::loadNodeLinks(OneIStream& stream)
{
    for (auto& node : nodes)
    {
        int ii;
        stream >> ii;
        for (int i = 0;i < ii;i++)
        {
            Element element;
            stream >> element.uv;
            int index;
            stream >> index;
            node->nextNodes[element] = links[index];
        }
    }
}

void OneRuleMap::generateInitOne(const char* fileName)
{
    OneOStream stream(fileName);
    saveStrings(stream);

    stream.startWriteRuleMap();
    saveRules(stream);
    saveNodes(stream);
    saveLinks(stream);
    saveNodeLinks(stream);
    stream.endWriteRuleMap();
}

void OneRuleMap::initOne()
{
    OneIStream stream(oneRuleMap);
    
    loadStrings(stream);
    loadRules(stream);
    ruleContainer.init(&block);
    loadNodes(stream);
    loadLinks(stream);
    loadNodeLinks(stream);
}
