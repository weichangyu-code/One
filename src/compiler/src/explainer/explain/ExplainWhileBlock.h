#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxWhileBlock.h"
#include "Common.h"

class ExplainWhileBlock : public ExplainBase
{
public:
    ExplainWhileBlock(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("whileblock", "", (MyRuleExecuteFunction)&onExplainWhileBlock);
        registe("dowhileblock", "", (MyRuleExecuteFunction)&onExplainDoWhileBlock);
        registe("break", "number", (MyRuleExecuteFunction)&onExplainBreak);
        registe("break", "", (MyRuleExecuteFunction)&onExplainBreak);
        registe("continue", "number", (MyRuleExecuteFunction)&onExplainContinue);
        registe("continue", "", (MyRuleExecuteFunction)&onExplainContinue);
    }
    
    Result onExplainWhileBlock(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxWhileBlock* block = new SyntaxWhileBlock(context);
        block->exp = (SyntaxExp*)es[2].ptr;
        block->block = (SyntaxBlock*)es[4].ptr;
        block->dowhile = false;
        out.ptr = block;
        return {};
    }
    
    Result onExplainDoWhileBlock(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxWhileBlock* block = new SyntaxWhileBlock(context);
        block->exp = (SyntaxExp*)es[4].ptr;
        block->block = (SyntaxBlock*)es[1].ptr;
        block->dowhile = true;
        out.ptr = block;
        return {};
    }

    Result onExplainBreak(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = new SyntaxExp(context);

        //TODO: number后续处理

        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = BREAK;
        exp->instructs.push_back(instruct);

        out.ptr = exp;
        return {};
    }

    Result onExplainContinue(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = new SyntaxExp(context);

        //TODO: number后续处理

        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = CONTINUE;
        exp->instructs.push_back(instruct);

        out.ptr = exp;
        return {};
    }
};