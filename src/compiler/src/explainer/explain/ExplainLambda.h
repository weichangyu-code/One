#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxMulti.h"
#include "../syntax/SyntaxBlock.h"
#include "../syntax/SyntaxClass.h"
#include "../syntax/SyntaxFunc.h"
#include "Common.h"

class ExplainLambda : public ExplainBase
{
public:
    ExplainLambda(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("lambda", "", (MyRuleExecuteFunction)&ExplainLambda::onExplainLambda);
        registe("lambda", "null", (MyRuleExecuteFunction)&ExplainLambda::onExplainLambdaNull);
        registe("newanony", "", (MyRuleExecuteFunction)&ExplainLambda::onExplainNewAnony);
    }

    Result onExplainLambda(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<string>* multi = (SyntaxMulti<string>*)es[1].ptr;
        SyntaxBlock* block = (SyntaxBlock*)es[3].ptr;

        //转换成一个new class
        SyntaxClass* clazz = new SyntaxClass(context);
        clazz->name = "";       //匿名为空，后续统一编号

        //生成一个匿名方法
        SyntaxFunc* func = new SyntaxFunc(context);
        func->name = "";        //匿名
        for (auto& name : multi->items)
        {
            SyntaxVarDef* varDef = new SyntaxVarDef(context);
            varDef->name = name;
            varDef->type = nullptr;          //类型需要后续判断
            func->params.push_back(varDef);
        }
        func->block = block;
        clazz->funcs.push_back(func);

        //
        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = NEW;
        instruct->type = new SyntaxType(context);
		instruct->type->anony = clazz;
        
		//new这个对象
        SyntaxExp* exp = new SyntaxExp(context);
        exp->instructs.push_back(instruct);
        exp->ret.setInstruct(instruct);

        out.ptr = exp;
        return {};
    }

    Result onExplainLambdaNull(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxBlock* block = (SyntaxBlock*)es[2].ptr;

        //转换成一个new class
        SyntaxClass* clazz = new SyntaxClass(context);
        clazz->isAnonyClass = true;

        //生成一个匿名方法
        SyntaxFunc* func = new SyntaxFunc(context);
        func->isAnony = true;
        func->block = block;
        clazz->funcs.push_back(func);

        //
        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = NEW;
        instruct->type = new SyntaxType(context);
		instruct->type->anony = clazz;
        
		//new这个对象
        SyntaxExp* exp = new SyntaxExp(context);
        exp->instructs.push_back(instruct);
        exp->ret.setInstruct(instruct);

        out.ptr = exp;
        return {};
    }

    Result onExplainNewAnony(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxType* type = (SyntaxType*)es[1].ptr;
        SyntaxMulti<SyntaxClassElement*>* multi = (SyntaxMulti<SyntaxClassElement*>*)es[2].ptr;

        //转换成一个new class
        SyntaxClass* clazz = new SyntaxClass(context);
        clazz->isAnonyClass = true;
        clazz->parents.push_back(type);

        //
        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = NEW;
        instruct->type = new SyntaxType(context);
		instruct->type->anony = clazz;
        
		//new这个对象
        SyntaxExp* exp = new SyntaxExp(context);
        exp->instructs.push_back(instruct);
        exp->ret.setInstruct(instruct);

        out.ptr = exp;
        return {};
    }
};
