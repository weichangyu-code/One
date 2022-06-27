#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxInstruct.h"
#include "../syntax/SyntaxExp.h"

class ExplainAssign : public ExplainBase
{
public:
    ExplainAssign(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
		registe("assign", "deepassign", (MyRuleExecuteFunction)&onExeDeepAssign);
		registe("assign", "refassign", (MyRuleExecuteFunction)&onExeRefAssign);

		registe("assign", "addassign", (MyRuleExecuteFunction)&onExeAddAssign);
		registe("assign", "subassign", (MyRuleExecuteFunction)&onExeSubAssign);
		registe("assign", "mulassign", (MyRuleExecuteFunction)&onExeMulAssign);
		registe("assign", "divassign", (MyRuleExecuteFunction)&onExeDivAssign);
		registe("assign", "recassign", (MyRuleExecuteFunction)&onExeRecAssign);
		registe("assign", "rbitmovassign", (MyRuleExecuteFunction)&onExeRbitmovAssign);
		registe("assign", "rbitmov2assign", (MyRuleExecuteFunction)&onExeRbitmov2Assign);
		registe("assign", "lbitmovassign", (MyRuleExecuteFunction)&onExeLbitmovAssign);
		registe("assign", "bitandassign", (MyRuleExecuteFunction)&onExeBitAndAssign);
		registe("assign", "bitxorassign", (MyRuleExecuteFunction)&onExeBitXorAssign);
		registe("assign", "bitorassign", (MyRuleExecuteFunction)&onExeBitOrAssign);
		registe("assign", "andassign", (MyRuleExecuteFunction)&onExeAndAssign);
		registe("assign", "orassign", (MyRuleExecuteFunction)&onExeOrAssign);
    }

    Result onExeRefAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
    }

	Result onExeDeepAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = DEEP_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeAddAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = ADD_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeSubAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = SUB_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeMulAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = MUL_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeDivAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = DIV_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeRecAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = REC_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeRbitmovAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = RBITMOV_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeRbitmov2Assign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = RBITMOV2_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeLbitmovAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = LBITMOV_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeBitAndAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = BITAND_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeBitXorAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = BITXOR_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeBitOrAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = BITOR_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeAndAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = AND_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeOrAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = OR_ASSIGN;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}
};
