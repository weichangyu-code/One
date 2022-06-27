
#pragma once
#include "ExplainContext.h"
#include "ExplainContainer.h"

typedef Result (ExplainBase::*MyRuleExecuteFunction)(Rule*, vector<LexElement>&, LexElement&);

class ExplainBase
{
public:
    ExplainBase(ExplainContainer* container, ExplainContext* context)
        :container(container), context(context)
    {}

protected:
    void registe(const string& name, const string& alias, MyRuleExecuteFunction func)
    {
        container->setRuleExecuteFuncion(name, alias, std::bind(func, this
            , std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

protected:
    ExplainContainer* container;
    ExplainContext* context;
};
