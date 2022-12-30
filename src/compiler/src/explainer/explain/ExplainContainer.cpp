#include "ExplainContainer.h"
#include "ExplainAssign.h"
#include "ExplainClass.h"
#include "ExplainCode.h"
#include "ExplainFile.h"
#include "ExplainFunc.h"
#include "ExplainIdentifier.h"
#include "ExplainType.h"
#include "ExplainVarDef.h"
#include "ExplainVar.h"
#include "ExplainExp.h"
#include "ExplainNew.h"
#include "ExplainPrefix.h"
#include "ExplainSentence.h"
#include "ExplainLambda.h"
#include "ExplainIfBlock.h"
#include "ExplainWhileBlock.h"
#include "ExplainForBlock.h"
#include "ExplainOperator.h"
#include "ExplainTryCatch.h"

ExplainContainer::ExplainContainer(RuleContainer* ruleContainer, ExplainContext* context)
    :ruleContainer(ruleContainer)
{
    this->funcs.resize(ruleContainer->block->rules.size());

    //添加解释器
    explains.push_back(new ExplainAssign(this, context));
    explains.push_back(new ExplainClass(this, context));
    explains.push_back(new ExplainCode(this, context));
    explains.push_back(new ExplainExp(this, context));
    explains.push_back(new ExplainFile(this, context));
    explains.push_back(new ExplainFunc(this, context));
    explains.push_back(new ExplainIdentifier(this, context));
    explains.push_back(new ExplainNew(this, context));
    explains.push_back(new ExplainPrefix(this, context));
    explains.push_back(new ExplainSentence(this, context));
    explains.push_back(new ExplainType(this, context));
    explains.push_back(new ExplainVarDef(this, context));
    explains.push_back(new ExplainVar(this, context));
    explains.push_back(new ExplainLambda(this, context));
    explains.push_back(new ExplainIfBlock(this, context));
    explains.push_back(new ExplainWhileBlock(this, context));
    explains.push_back(new ExplainForBlock(this, context));
    explains.push_back(new ExplainTryCatch(this, context));
    
    //不支持操作符重载
    //explains.push_back(new ExplainOperator(this, context));
}

ExplainContainer::~ExplainContainer()
{
    for (auto explain : explains)
    {
        delete explain;
    }
}

void ExplainContainer::setRuleExecuteFuncion(const string& ruleName, const string& alias, RuleExecuteFunction func)
{
    list<Rule*> rules = ruleContainer->getRules(ruleName);
    for (auto& rule : rules)
    {
        if (alias.empty())
        {
            //只填写没被填写过的
            if (funcs[rule->index] == nullptr)
            {
                funcs[rule->index] = func; 
            }
        }

        //
        if (rule->alias == alias)
        {
            funcs[rule->index] = func;
        }
    }
}
    
Result ExplainContainer::callRuleExecuteFunction(Rule* rule, vector<LexElement>& es, LexElement& out)
{
    RuleExecuteFunction& func = funcs[rule->index];
    if (func)
    {
        return func(rule, es, out);
    }
    else
    {
        return {};
    }
}
