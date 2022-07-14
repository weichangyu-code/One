#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxVarDef.h"
#include "../syntax/SyntaxType.h"
#include "../syntax/SyntaxExp.h"
#include "../syntax/SyntaxMulti.h"

class ExplainVarDef : public ExplainBase
{
public:
    ExplainVarDef(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("vardefstart", "auto", (MyRuleExecuteFunction)&onExplainVarDefStartAuto);
        registe("vardefstart", "new", (MyRuleExecuteFunction)&onExplainVarDefStartNew);
        registe("vardefstart", "assign", (MyRuleExecuteFunction)&onExplainVarDefStartAssign);
        registe("vardefstart", "array", (MyRuleExecuteFunction)&onExplainVarDefStartArray);
        registe("vardefstart", "normal", (MyRuleExecuteFunction)&onExplainVarDefStartNormal);
        registe("vardef", "prefix", (MyRuleExecuteFunction)&onExplainVarDefPrefix);

        registe("vardefnull", "auto", (MyRuleExecuteFunction)&onExplainVarDefNullAuto);
        registe("vardefnull", "normal", (MyRuleExecuteFunction)&onExplainVarDefNullNormal);
        
        registe("vardeftypestart", "new", (MyRuleExecuteFunction)&onExplainVarDefTypeStartNew);
        registe("vardeftypestart", "normal", (MyRuleExecuteFunction)&onExplainVarDefTypeStartNormal);
        registe("vardeftypestart", "assign", (MyRuleExecuteFunction)&onExplainVarDefTypeStartAssign);
        registe("vardeftypestart", "array", (MyRuleExecuteFunction)&onExplainVarDefTypeStartArray);
        registe("vardeftype", "prefix", (MyRuleExecuteFunction)&onExplainVarDefTypePrefix);
    }

    Result onExplainVarDefStartAuto(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[3].ptr;

        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[1].remark;
        varDef->exp = exp;

        SyntaxInstruct* instructLeft = new SyntaxInstruct(context);
        instructLeft->cmd = VARDEF;
        instructLeft->varDef = varDef;
        exp->instructs.push_back(instructLeft);

        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = ASSIGN;
        instruct->params.push_back(instructLeft);
        instruct->params.push_back(exp->ret);
        exp->instructs.push_back(instruct);
        exp->ret.setInstruct(instruct);
        
        out.ptr = varDef;
        return {};
    }

    Result onExplainVarDefStartNew(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxType* type = (SyntaxType*)es[0].ptr;
        SyntaxMulti<SyntaxExp*>* multiExp = (SyntaxMulti<SyntaxExp*>*)es[2].ptr;

        SyntaxExp* exp = new SyntaxExp(context);
        for (auto& expTmp : multiExp->items)
        {
            exp->append(expTmp);
        }

        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[1].remark;
        varDef->type = type;
        varDef->exp = exp;

        SyntaxInstruct* instructRight = new SyntaxInstruct(context);
        instructRight->cmd = NEW;
        instructRight->type = varDef->type;
        for (auto& expTmp : multiExp->items)
        {
            instructRight->params.push_back(expTmp->ret);
        }
        exp->instructs.push_back(instructRight);

        SyntaxInstruct* instructLeft = new SyntaxInstruct(context);
        instructLeft->cmd = VARDEF;
        instructLeft->varDef = varDef;
        exp->instructs.push_back(instructLeft);

        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = ASSIGN;
        instruct->params.push_back(instructLeft);
        instruct->params.push_back(instructRight);
        exp->instructs.push_back(instruct);
        exp->ret.setInstruct(instruct);
        
        out.ptr = varDef;
        return {};
    }

    Result onExplainVarDefStartAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[3].ptr;

        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[1].remark;
        varDef->type = (SyntaxType*)es[0].ptr;
        varDef->exp = exp;

        SyntaxInstruct* instructLeft = new SyntaxInstruct(context);
        instructLeft->cmd = VARDEF;
        instructLeft->varDef = varDef;
        exp->instructs.push_back(instructLeft);

        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = ASSIGN;
        instruct->params.push_back(instructLeft);
        instruct->params.push_back(exp->ret);
        exp->instructs.push_back(instruct);
        exp->ret.setInstruct(instruct);
        
        out.ptr = varDef;
        return {};
    }

    Result onExplainVarDefStartArray(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[3].ptr;
        SyntaxType* type = (SyntaxType*)es[0].ptr;

        SyntaxType* arrType = new SyntaxType(context);
        arrType->items = type->items;
        arrType->arrNum = type->arrNum + 1;

        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[1].remark;
        varDef->type = arrType;
        varDef->exp = exp;
        
        SyntaxInstruct* instructRight = new SyntaxInstruct(context);
        instructRight->cmd = NEW_ARRAY;
        instructRight->type = type;          //类型已经数组加1了
        instructRight->params.push_back(exp->ret);
        exp->instructs.push_back(instructRight);

        //
        SyntaxInstruct* instructLeft = new SyntaxInstruct(context);
        instructLeft->cmd = VARDEF;
        instructLeft->varDef = varDef;
        exp->instructs.push_back(instructLeft);

        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = ASSIGN;
        instruct->params.push_back(instructLeft);
        instruct->params.push_back(instructRight);
        exp->instructs.push_back(instruct);
        exp->ret.setInstruct(instruct);
        
        out.ptr = varDef;
        return {};
    }

    Result onExplainVarDefStartNormal(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = new SyntaxExp(context);

        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[1].remark;
        varDef->type = (SyntaxType*)es[0].ptr;
        varDef->exp = exp;

        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = VARDEF;
        instruct->varDef = varDef;
        exp->instructs.push_back(instruct);
        exp->ret.setInstruct(instruct);
        
        out.ptr = varDef;
        return {};
    }

    Result onExplainVarDefPrefix(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVarDef* varDef = (SyntaxVarDef*)es[1].ptr;
        SyntaxMulti<string>* prefix = (SyntaxMulti<string>*)es[0].ptr;
        varDef->isConst = prefix->exist(KEY_CONST);
        varDef->isStatic = prefix->exist(KEY_STATIC);

        out.ptr = varDef;
        return {};
    }

    Result onExplainVarDefNullAuto(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = new SyntaxExp(context);

        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[1].remark;
        varDef->exp = exp;

        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = VARDEF;
        instruct->varDef = varDef;
        exp->instructs.push_back(instruct);
        exp->ret.setInstruct(instruct);

        out.ptr = varDef;
        return {};
    }

    Result onExplainVarDefNullNormal(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return onExplainVarDefStartNormal(rule, es, out);
    }
    
    Result onExplainVarDefTypeStartNew(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return onExplainVarDefStartNew(rule, es, out);
    }

    Result onExplainVarDefTypeStartAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return onExplainVarDefStartAssign(rule, es, out);
    }

    Result onExplainVarDefTypeStartArray(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return onExplainVarDefStartArray(rule, es, out);
    }

    Result onExplainVarDefTypeStartNormal(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return onExplainVarDefStartNormal(rule, es, out);
    }

    Result onExplainVarDefTypePrefix(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return onExplainVarDefPrefix(rule, es, out);
    }
};
