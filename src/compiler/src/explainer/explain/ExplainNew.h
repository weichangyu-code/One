#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxMulti.h"
#include "../syntax/SyntaxExp.h"
#include "../syntax/SyntaxInstruct.h"
#include "../syntax/SyntaxType.h"

class ExplainNew : public ExplainBase
{
public:
    ExplainNew(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
		registe("newobject", "param", (MyRuleExecuteFunction)&onExeNewObjectParam);
		registe("newobject", "", (MyRuleExecuteFunction)&onExeNewObject);
		registe("newarray", "", (MyRuleExecuteFunction)&onExeNewArray);
    }

	Result onExeNewObjectParam(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
        SyntaxType* type = (SyntaxType*)es[1].ptr;
        SyntaxMulti<SyntaxExp*>* multiExp = (SyntaxMulti<SyntaxExp*>*)es[2].ptr;

        SyntaxExp* exp = new SyntaxExp(context);
        for (auto& expTmp : multiExp->items)
        {
            exp->append(expTmp);
        }
        
        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = NEW;
        instruct->type = type;
        for (auto& expTmp : multiExp->items)
        {
            instruct->params.push_back(expTmp->ret);
        }
        exp->instructs.push_back(instruct);
        exp->ret.setInstruct(instruct);

        out.ptr = exp;
		return {};
	}
    
	Result onExeNewObject(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
        SyntaxType* type = (SyntaxType*)es[1].ptr;
        SyntaxExp* exp = new SyntaxExp(context);
        
        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = NEW;
        instruct->type = type;
        exp->instructs.push_back(instruct);
        exp->ret.setInstruct(instruct);

        out.ptr = exp;
		return {};
	}

	Result onExeNewArray(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
        SyntaxType* type = (SyntaxType*)es[1].ptr;
        SyntaxExp* exp = (SyntaxExp*)es[3].ptr;
        
        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = NEW_ARRAY;
        instruct->type = type;
        instruct->params.push_back(exp->ret);
        exp->instructs.push_back(instruct);
        exp->ret.setInstruct(instruct);

        out.ptr = exp;
		return {};
	}
};
