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
        registe("vardefitemautoassign", "", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefItemAutoAssign);
        registe("vardefitemauto", "", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefItemAuto);
        //registe("vardefitemnew", "", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefItemNew);
        registe("vardefitemassign", "", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefItemAssign);
        //registe("vardefitemarray", "", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefItemArray);
        registe("vardefitemtype", "", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefItemType);
        registe("vardefitemdot", "", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefItemDot);

        registe("vardefsentence", "prefix", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefPrefix);
        registe("vardefclass", "prefix", (MyRuleExecuteFunction)&ExplainVarDef::onExplainVarDefPrefix);
    }

    Result onExplainVarDefItemAutoAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[3].ptr;

        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[1].remark;
        varDef->exp = exp;
        varDef->deepAssign = es[2].remark != "=";

        out.ptr = varDef;
        return {};
    }

    // Result onExplainVarDefItemNew(Rule* rule, vector<LexElement>& es, LexElement& out)
    // {
    //     SyntaxType* type = (SyntaxType*)es[0].ptr;
    //     SyntaxMulti<SyntaxExp*>* multiExp = (SyntaxMulti<SyntaxExp*>*)es[2].ptr;

    //     SyntaxExp* exp = new SyntaxExp(context);
    //     for (auto& expTmp : multiExp->items)
    //     {
    //         exp->append(expTmp);
    //     }

    //     SyntaxVarDef* varDef = new SyntaxVarDef(context);
    //     varDef->name = es[1].remark;
    //     varDef->type = type;
    //     varDef->exp = exp;

    //     SyntaxInstruct* instructRight = new SyntaxInstruct(context);
    //     instructRight->cmd = NEW;
    //     instructRight->type = varDef->type;
    //     for (auto& expTmp : multiExp->items)
    //     {
    //         instructRight->params.push_back(expTmp->ret);
    //     }
    //     exp->instructs.push_back(instructRight);

    //     exp->ret.setInstruct(instructRight);
        
    //     out.ptr = varDef;
    //     return {};
    // }

    Result onExplainVarDefItemAssign(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[3].ptr;

        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[1].remark;
        varDef->type = (SyntaxType*)es[0].ptr;
        varDef->exp = exp;
        varDef->deepAssign = es[2].remark != "=";

        out.ptr = varDef;
        return {};
    }

    // Result onExplainVarDefItemArray(Rule* rule, vector<LexElement>& es, LexElement& out)
    // {
    //     SyntaxExp* exp = (SyntaxExp*)es[3].ptr;
    //     SyntaxType* type = (SyntaxType*)es[0].ptr;

    //     SyntaxType* arrType = new SyntaxType(context);
    //     arrType->items = type->items;
    //     arrType->arrNum = type->arrNum + 1;

    //     SyntaxVarDef* varDef = new SyntaxVarDef(context);
    //     varDef->name = es[1].remark;
    //     varDef->type = arrType;
    //     varDef->exp = exp;
        
    //     SyntaxInstruct* instructRight = new SyntaxInstruct(context);
    //     instructRight->cmd = NEW_ARRAY;
    //     instructRight->type = type;          //类型已经数组加1了
    //     instructRight->params.push_back(exp->ret);
    //     exp->instructs.push_back(instructRight);

    //     exp->ret.setInstruct(instructRight);
        
    //     out.ptr = varDef;
    //     return {};
    // }

    Result onExplainVarDefItemType(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[1].remark;
        varDef->type = (SyntaxType*)es[0].ptr;

        out.ptr = varDef;
        return {};
    }

    Result onExplainVarDefItemDot(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[2].remark;
        varDef->type = (SyntaxType*)es[0].ptr;
        varDef->type->ellipsis = true;                             //变成一个数组
        varDef->isDynamic = true;

        out.ptr = varDef;
        return {};
    }

    Result onExplainVarDefItemAuto(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVarDef* varDef = new SyntaxVarDef(context);
        varDef->name = es[1].remark;

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
};
