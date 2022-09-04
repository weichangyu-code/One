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
        registe("vardefstart", "auto", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefStartAuto);
        registe("vardefstart", "new", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefStartNew);
        registe("vardefstart", "assign", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefStartAssign);
        registe("vardefstart", "array", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefStartArray);
        registe("vardefstart", "normal", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefStartNormal);
        registe("vardef", "prefix", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefPrefix);

        registe("vardefnull", "auto", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefNullAuto);
        registe("vardefnull", "normal", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefNullNormal);
        
        registe("vardeftypestart", "new", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefTypeStartNew);
        registe("vardeftypestart", "normal", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefTypeStartNormal);
        registe("vardeftypestart", "assign", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefTypeStartAssign);
        registe("vardeftypestart", "array", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefTypeStartArray);
        registe("vardeftype", "prefix", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefTypePrefix);
    }

    Result onExplainVarDefStartAuto(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[3].ptr;

        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[1].remark;
        varDef->exp = exp;

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

        exp->ret.setInstruct(instructRight);
        
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

        exp->ret.setInstruct(instructRight);
        
        out.ptr = varDef;
        return {};
    }

    Result onExplainVarDefStartNormal(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[1].remark;
        varDef->type = (SyntaxType*)es[0].ptr;

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
        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[1].remark;

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
