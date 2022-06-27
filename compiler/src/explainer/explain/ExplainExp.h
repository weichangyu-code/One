#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxConst.h"
#include "../syntax/SyntaxExp.h"
#include "../syntax/SyntaxInstruct.h"
#include "../syntax/SyntaxVar.h"
#include "../syntax/SyntaxVarDef.h"
#include "../syntax/SyntaxType.h"
#include "../syntax/SyntaxData.h"
#include "../syntax/SyntaxMulti.h"

class ExplainExp : public ExplainBase
{
public:
    ExplainExp(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
		registe("exp", "number", (MyRuleExecuteFunction)&onExeExpNumber);
		registe("exp", "string", (MyRuleExecuteFunction)&onExeExpString);
		registe("exp", "char", (MyRuleExecuteFunction)&onExeExpChar);
		registe("exp", "boolean", (MyRuleExecuteFunction)&onExeExpBoolean);
		registe("exp", "null", (MyRuleExecuteFunction)&onExeExpNull);

		registe("exp", "multishortsentence", (MyRuleExecuteFunction)&onExeExpMultiShortSentence);
		registe("exp", "exp", (MyRuleExecuteFunction)&onExeExpExp);

		registe("exp", "callfunc", (MyRuleExecuteFunction)&onExeExpCallFunc);
		registe("exp", "var", (MyRuleExecuteFunction)&onExeExpVar);

		registe("exp", "forcetype", (MyRuleExecuteFunction)&onExeExpForceType);

		registe("exp", "index", (MyRuleExecuteFunction)&onExeExpIndex);

		registe("exp", "linc", (MyRuleExecuteFunction)&onExeExpLInc);
		registe("exp", "ldec", (MyRuleExecuteFunction)&onExeExpLDec);
		registe("exp", "rinc", (MyRuleExecuteFunction)&onExeExpRInc);
		registe("exp", "rdec", (MyRuleExecuteFunction)&onExeExpRDec);

		registe("exp", "positive", (MyRuleExecuteFunction)&onExeExpPositive);
		registe("exp", "negative", (MyRuleExecuteFunction)&onExeExpNegative);

		registe("exp", "not", (MyRuleExecuteFunction)&onExeExpNot);
		registe("exp", "bitnot", (MyRuleExecuteFunction)&onExeExpBitNot);

		registe("exp", "mul", (MyRuleExecuteFunction)&onExeExpMul);
		registe("exp", "div", (MyRuleExecuteFunction)&onExeExpDiv);
		registe("exp", "rec", (MyRuleExecuteFunction)&onExeExpRec);

		registe("exp", "add", (MyRuleExecuteFunction)&onExeExpAdd);
		registe("exp", "sub", (MyRuleExecuteFunction)&onExeExpSub);

		registe("exp", "rbitmov", (MyRuleExecuteFunction)&onExeExpRbitmov);
		registe("exp", "rbitmov2", (MyRuleExecuteFunction)&onExeExpRbitmov2);
		registe("exp", "lbitmov", (MyRuleExecuteFunction)&onExeExpLbitmov);

		registe("exp", "gt", (MyRuleExecuteFunction)&onExeExpGT);
		registe("exp", "gte", (MyRuleExecuteFunction)&onExeExpGTE);
		registe("exp", "lt", (MyRuleExecuteFunction)&onExeExpLT);
		registe("exp", "lte", (MyRuleExecuteFunction)&onExeExpLTE);

		registe("exp", "eq", (MyRuleExecuteFunction)&onExeExpEQ);
		registe("exp", "neq", (MyRuleExecuteFunction)&onExeExpNEQ);
		registe("exp", "eqd", (MyRuleExecuteFunction)&onExeExpEQDeep);
		registe("exp", "neqd", (MyRuleExecuteFunction)&onExeExpNEQDeep);

		registe("exp", "bitand", (MyRuleExecuteFunction)&onExeExpBitAnd);
		registe("exp", "bitxor", (MyRuleExecuteFunction)&onExeExpBitXor);
		registe("exp", "bitor", (MyRuleExecuteFunction)&onExeExpBitOr);

		registe("exp", "and", (MyRuleExecuteFunction)&onExeExpAnd);
		registe("exp", "or", (MyRuleExecuteFunction)&onExeExpOr);

		registe("exp", "cond", (MyRuleExecuteFunction)&onExeExpCond);

		registe("exp", "assign", (MyRuleExecuteFunction)&onExeExpAssign);

		registe("multiexp", "", (MyRuleExecuteFunction)&onExeMultiExp);
		registe("multiexp", "add", (MyRuleExecuteFunction)&onExeMultiExpAdd);
    }

	Result onExeExpNumber(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
        SyntaxExp* exp = new SyntaxExp(context);
        exp->ret.setConst(new SyntaxConst(context, SyntaxConst::NUMBER, es[0].remark));
        out.ptr = exp;
		return {};
	}
	Result onExeExpString(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
        SyntaxExp* exp = new SyntaxExp(context);
        exp->ret.setConst(new SyntaxConst(context, SyntaxConst::STRING, es[0].remark));
        out.ptr = exp;
		return {};
	}

	Result onExeExpChar(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
        SyntaxExp* exp = new SyntaxExp(context);
        exp->ret.setConst(new SyntaxConst(context, SyntaxConst::CHAR, es[0].remark));
        out.ptr = exp;
		return {};
	}

	Result onExeExpBoolean(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
        SyntaxExp* exp = new SyntaxExp(context);
        exp->ret.setConst(new SyntaxConst(context, SyntaxConst::BOOLEAN, es[0].remark));
        out.ptr = exp;
		return {};
	}

	Result onExeExpNull(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
        SyntaxExp* exp = new SyntaxExp(context);
        exp->ret.setConst(new SyntaxConst(context, SyntaxConst::REFNULL, es[0].remark));
        out.ptr = exp;
		return {};
	}

	Result onExeExpMultiShortSentence(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[1].ptr;
        out.ptr = exp;
		return {};
	}

	Result onExeExpExp(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[1].ptr;
        out.ptr = exp;
		return {};
	}

	Result onExeExpCallFunc(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		return {};
	}

	Result onExeExpVar(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxVar* var = (SyntaxVar*)es[0].ptr;

        SyntaxExp* exp = new SyntaxExp(context);
		exp->ret.setVar(var);
        out.ptr = exp;
		return {};
	}

	Result onExeExpForceType(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxType* type = (SyntaxType*)es[1].ptr;
		SyntaxExp* exp = (SyntaxExp*)es[3].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = TYPE;
		instruct->type = type;

		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpIndex(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* expIndex = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = INDEX;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(expIndex->ret);

		exp->append(expIndex);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpLInc(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[1].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = LINC;
		instruct->params.push_back(exp->ret);

		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpLDec(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[1].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = LDEC;
		instruct->params.push_back(exp->ret);

		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpRInc(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = RINC;
		instruct->params.push_back(exp->ret);

		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpRDec(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = RDEC;
		instruct->params.push_back(exp->ret);

		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpPositive(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[1].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = POS;
		instruct->params.push_back(exp->ret);

		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpNegative(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[1].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = NEG;
		instruct->params.push_back(exp->ret);

		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpNot(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[1].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = NOT;
		instruct->params.push_back(exp->ret);

		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpBitNot(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[1].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = BITNOT;
		instruct->params.push_back(exp->ret);

		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpMul(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = MUL;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpDiv(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = DIV;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpRec(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = REC;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = ADD;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpSub(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = SUB;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpRbitmov(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = RBITMOV;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpRbitmov2(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = RBITMOV2;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpLbitmov(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = LBITMOV;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpGT(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = GT;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpGTE(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = GTE;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpLT(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = LT;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpLTE(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = LTE;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpEQ(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = EQ;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpNEQ(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = NEQ;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpEQDeep(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = EQ_DEEP;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpNEQDeep(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = NEQ_DEEP;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpBitAnd(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = BITAND;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpBitXor(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = BITXOR;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpBitOr(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = BITOR;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};
	}

	Result onExeExpAnd(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = AND;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};

		// SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		// SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		// //新建变量，用于存储结果
		// SyntaxVarDef* tmp = new SyntaxVarDef(context);
		// tmp->type = SyntaxType::createBoolType(context);

		// //跳转定位用的空指令，没啥用
		// SyntaxInstruct* label = new SyntaxInstruct(context);
		// label->cmd = LABEL;

		// //先赋值给临时变量
		// SyntaxInstruct* assign = new SyntaxInstruct(context);
		// assign->cmd = ASSIGN;
		// assign->params.push_back(tmp);
		// assign->params.push_back(exp->ret);
		// exp->instructs.push_back(assign);

		// //跳转指令
		// SyntaxInstruct* jump = new SyntaxInstruct(context);
		// jump->cmd = JUMPF;
		// jump->params.push_back(tmp);
		// jump->jump = label;

		// //添加右边
		// exp->append(exp2);

		// //右边结果赋值
		// assign = new SyntaxInstruct(context);
		// assign->cmd = ASSIGN;
		// assign->params.push_back(tmp);
		// assign->params.push_back(exp2->ret);
		// exp->instructs.push_back(assign);

		// //添加LABEL
		// exp->instructs.push_back(label);

		// exp->ret.setVarDef(tmp);
		// out.ptr = exp;
		// return {};
	}

	Result onExeExpOr(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = OR;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};

		// SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		// SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

		// //新建变量，用于存储结果
		// SyntaxVarDef* tmp = new SyntaxVarDef(context);
		// tmp->type = SyntaxType::createBoolType(context);

		// //跳转定位用的空指令，没啥用
		// SyntaxInstruct* label = new SyntaxInstruct(context);
		// label->cmd = LABEL;

		// //先赋值给临时变量
		// SyntaxInstruct* assign = new SyntaxInstruct(context);
		// assign->cmd = ASSIGN;
		// assign->params.push_back(tmp);
		// assign->params.push_back(exp->ret);
		// exp->instructs.push_back(assign);

		// //跳转指令
		// SyntaxInstruct* jump = new SyntaxInstruct(context);
		// jump->cmd = JUMPT;
		// jump->params.push_back(tmp);
		// jump->jump = label;

		// //添加右边
		// exp->append(exp2);

		// //右边结果赋值
		// assign = new SyntaxInstruct(context);
		// assign->cmd = ASSIGN;
		// assign->params.push_back(tmp);
		// assign->params.push_back(exp2->ret);
		// exp->instructs.push_back(assign);

		// //添加LABEL
		// exp->instructs.push_back(label);

		// exp->ret.setVarDef(tmp);
		// out.ptr = exp;
		// return {};
	}

	Result onExeExpCond(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;
		SyntaxExp* exp3 = (SyntaxExp*)es[4].ptr;

		SyntaxInstruct* instruct = new SyntaxInstruct(context);
		instruct->cmd = COND;
		instruct->params.push_back(exp->ret);
		instruct->params.push_back(exp2->ret);
		instruct->params.push_back(exp3->ret);

		exp->append(exp2);
		exp->instructs.push_back(instruct);
		exp->ret.setInstruct(instruct);
        out.ptr = exp;
		return {};

		// SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
		// SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;
		// SyntaxExp* exp3 = (SyntaxExp*)es[4].ptr;

		// //新建变量，用于存储结果。类型自动
		// SyntaxVarDef* tmp = new SyntaxVarDef(context);

		// //跳转定位用的空指令，没啥用
		// SyntaxInstruct* label1 = new SyntaxInstruct(context);
		// label1->cmd = LABEL;
		// SyntaxInstruct* label2 = new SyntaxInstruct(context);
		// label2->cmd = LABEL;
		
		// //处理EXP2
		// SyntaxInstruct* jump = new SyntaxInstruct(context);
		// jump->cmd = JUMPF;
		// jump->params.push_back(exp->ret);
		// jump->jump = label1;
		// exp->instructs.push_back(jump);

		// exp->append(exp2);

		// SyntaxInstruct* assign = new SyntaxInstruct(context);
		// assign->cmd = ASSIGN;
		// assign->params.push_back(tmp);
		// assign->params.push_back(exp2->ret);
		// exp->instructs.push_back(assign);

		// jump = new SyntaxInstruct(context);
		// jump->cmd = JUMP;
		// jump->jump = label2;
		// exp->instructs.push_back(jump);

		// //处理EXP3
		// exp->instructs.push_back(label1);

		// exp->append(exp3);

		// assign = new SyntaxInstruct(context);
		// assign->cmd = ASSIGN;
		// assign->params.push_back(tmp);
		// assign->params.push_back(exp3->ret);
		// exp->instructs.push_back(assign);

		// exp->instructs.push_back(label2);

		// exp->ret.setVarDef(tmp);
		// out.ptr = exp;
		// return {};
	}

	Result onExeExpAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		return {};
	}

	Result onExeMultiExp(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;

		SyntaxMulti<SyntaxExp*>* multiExp = new SyntaxMulti<SyntaxExp*>(context);
		multiExp->items.push_back(exp);

		out.ptr = multiExp;
		return {};
	}

	Result onExeMultiExpAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
	{
		SyntaxMulti<SyntaxExp*>* multiExp = (SyntaxMulti<SyntaxExp*>*)es[0].ptr;
		SyntaxExp* exp = (SyntaxExp*)es[2].ptr;

		multiExp->items.push_back(exp);

		out.ptr = multiExp;
		return {};
	}
};

