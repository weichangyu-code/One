#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxFunc.h"
#include "../syntax/SyntaxMulti.h"
#include "../syntax/SyntaxVar.h"
#include "../syntax/SyntaxType.h"

class ExplainFunc : public ExplainBase
{
public:
    ExplainFunc(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("multifuncparamdef", "", (MyRuleExecuteFunction)&onExplainMultiFuncParamDef);
        registe("multifuncparamdef", "add", (MyRuleExecuteFunction)&onExplainMultiFuncParamDefAdd);

        registe("funcdefparam", "", (MyRuleExecuteFunction)&onExplainFuncDefParam);
        registe("funcdefparam", "null", (MyRuleExecuteFunction)&onExplainFuncDefParamNull);
        registe("funcdefname", "identifier", (MyRuleExecuteFunction)&onExplainFuncDefName);
        //registe("funcdefname", "templatedef", (MyRuleExecuteFunction)&onExplainFuncDefTemplate);
        registe("funcdefheader", "", (MyRuleExecuteFunction)&onExplainFuncDefHeader);
        registe("funcdef", "normal", (MyRuleExecuteFunction)&onExplainFuncDefNormal);
        registe("funcdef", "static", (MyRuleExecuteFunction)&onExplainFuncDefStatic);
        registe("funcdef", "vnormal", (MyRuleExecuteFunction)&onExplainFuncDefVirtualNormal);
        registe("funcdef", "virtual", (MyRuleExecuteFunction)&onExplainFuncDefVirtual);

        registe("constructdef", "", (MyRuleExecuteFunction)&onExplainConstructDef);
        registe("destructdef", "", (MyRuleExecuteFunction)&onExplainDestructDef);

        registe("callfuncparam", "null", (MyRuleExecuteFunction)&onExplainCallFuncParamNull);
        registe("callfuncparam", "", (MyRuleExecuteFunction)&onExplainCallFuncParam);
        registe("callfunc", "destruct", (MyRuleExecuteFunction)&onExplainCallFuncDestruct);
        registe("callfunc", "", (MyRuleExecuteFunction)&onExplainCallFunc);

        registe("return", "", (MyRuleExecuteFunction)&onExplainReturn);
        registe("return", "null", (MyRuleExecuteFunction)&onExplainReturnNull);
    }
    
    Result onExplainMultiFuncParamDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxVarDef*>* multiVarDef = new SyntaxMulti<SyntaxVarDef*>(context);
        SyntaxVarDef* varDef = (SyntaxVarDef*)es[0].ptr;

        multiVarDef->items.push_back(varDef);
        
        out.ptr = multiVarDef;
        return {};
    }
    
    Result onExplainMultiFuncParamDefAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxVarDef*>* multiVarDef = (SyntaxMulti<SyntaxVarDef*>*)es[0].ptr;
        SyntaxVarDef* varDef = (SyntaxVarDef*)es[1].ptr;

        multiVarDef->items.push_back(varDef);
        
        out.ptr = multiVarDef;
        return {};
    }
    
    
    Result onExplainFuncDefParam(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxVarDef*>* multiVarDef = (SyntaxMulti<SyntaxVarDef*>*)es[1].ptr;
        
        out.ptr = multiVarDef;
        return {};
    }
    
    
    Result onExplainFuncDefParamNull(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxVarDef*>* multiVarDef = new SyntaxMulti<SyntaxVarDef*>(context);

        out.ptr = multiVarDef;
        return {};
    }
    
    Result onExplainFuncDefName(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxFunc* func = new SyntaxFunc(context);
        func->name = es[0].remark;

        out.ptr = func;
        return {};
    }
    
    // Result onExplainFuncDefTemplate(Rule* rule, vector<LexElement>& es, LexElement& out)
    // {
    //     SyntaxFunc* func = new SyntaxFunc(context);
    //     func->name = es[0].remark;

    //     //TODO:模板

    //     out.ptr = func;
    //     return {};
    // }
    
    Result onExplainFuncDefHeader(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxFunc* func = (SyntaxFunc*)es[1].ptr;
        func->return_ = (SyntaxType*)es[0].ptr;
        func->params = std::move(((SyntaxMulti<SyntaxVarDef*>*)es[2].ptr)->items);

        out.ptr = func;
        return {};
    }
    
    Result onExplainFuncDefNormal(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxFunc* func = (SyntaxFunc*)es[0].ptr;
        SyntaxBlock* block = (SyntaxBlock*)es[1].ptr;

        func->block = block;

        out.ptr = func;
        return {};
    }
    
    Result onExplainFuncDefStatic(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxFunc* func = (SyntaxFunc*)es[1].ptr;
        SyntaxBlock* block = (SyntaxBlock*)es[2].ptr;

        func->isStatic = true;
        func->block = block;

        out.ptr = func;
        return {};
    }
    
    Result onExplainFuncDefVirtualNormal(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxFunc* func = (SyntaxFunc*)es[1].ptr;
        SyntaxBlock* block = (SyntaxBlock*)es[2].ptr;

        func->isVirtual = true;
        func->block = block;

        out.ptr = func;
        return {};
    }
    
    Result onExplainFuncDefVirtual(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxFunc* func = (SyntaxFunc*)es[1].ptr;
        func->isVirtual = true;
        out.ptr = func;
        return {};
    }
    
    Result onExplainConstructDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxBlock* block = (SyntaxBlock*)es[2].ptr;
        SyntaxMulti<SyntaxVarDef*>* multiVarDef = (SyntaxMulti<SyntaxVarDef*>*)es[1].ptr;

        SyntaxFunc* func = new SyntaxFunc(context);
        func->type = FUNC_CONSTRUCT;
        func->name = es[0].remark;
        func->params = std::move(multiVarDef->items);
        func->block = block;

        out.ptr = func;
        return {};
    }
    
    Result onExplainDestructDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxBlock* block = (SyntaxBlock*)es[4].ptr;

        SyntaxFunc* func = new SyntaxFunc(context);
        func->type = FUNC_DESTRUCT;
        func->isVirtual = true;
        func->name = "~" + es[1].remark;
        func->block = block;

        out.ptr = func;
        return {};
    }

    Result onExplainCallFuncParamNull(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxExp*>* multiExp = new SyntaxMulti<SyntaxExp*>(context);
        
        out.ptr = multiExp;
        return {};
    }
    
    Result onExplainCallFuncParam(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxExp*>* multiExp = (SyntaxMulti<SyntaxExp*>*)es[1].ptr;
        
        out.ptr = multiExp;
        return {};
    }

    Result onExplainCallFuncDestruct(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVar* var = (SyntaxVar*)es[0].ptr;
        
        SyntaxTypePathItem* item = new SyntaxTypePathItem(context);
        item->typeName = "~";
        var->items.push_back(item);
        
        SyntaxExp* exp = new SyntaxExp(context);

        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = CALL;
        instruct->func = var;
        exp->instructs.push_back(instruct);

        out.ptr = exp;
        return {};
    }

    Result onExplainCallFunc(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVar* var = (SyntaxVar*)es[0].ptr;
        SyntaxMulti<SyntaxExp*>* multiExp = (SyntaxMulti<SyntaxExp*>*)es[1].ptr;
        
        SyntaxExp* exp = new SyntaxExp(context);
        for (auto& expTmp : multiExp->items)
        {
            exp->append(expTmp);
        }

        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = CALL;
        instruct->func = var;
        for (auto& expTmp : multiExp->items)
        {
            instruct->params.push_back(expTmp->ret);
        }
        exp->instructs.push_back(instruct);

        out.ptr = exp;
        return {};
    }

    Result onExplainReturn(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[1].ptr;

        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = RETURN;
        instruct->params.push_back(exp->ret);
        exp->instructs.push_back(instruct);
        exp->ret = exp->ret;

        out.ptr = exp;
        return {};
    }

    Result onExplainReturnNull(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = new SyntaxExp(context);

        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = RETURN;
        exp->instructs.push_back(instruct);

        out.ptr = exp;
        return {};
    }

};