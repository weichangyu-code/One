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
        registe("whileblock", "", (MyRuleExecuteFunction)&ExplainWhileBlock::onExplainWhileBlock);
        registe("dowhileblock", "", (MyRuleExecuteFunction)&ExplainWhileBlock::onExplainDoWhileBlock);
        registe("break", "number", (MyRuleExecuteFunction)&ExplainWhileBlock::onExplainBreak);
        registe("break", "", (MyRuleExecuteFunction)&ExplainWhileBlock::onExplainBreak);
        registe("continue", "number", (MyRuleExecuteFunction)&ExplainWhileBlock::onExplainContinue);
        registe("continue", "", (MyRuleExecuteFunction)&ExplainWhileBlock::onExplainContinue);
    }
    
    Result onExplainWhileBlock(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxWhileBlock* block = new SyntaxWhileBlock(context);
        block->exp = (SyntaxExp*)es[2].ptr;
        block->block = (SyntaxBlock*)es[4].ptr;
        block->type = SyntaxWhileBlock::WHILE;
        out.ptr = block;
        return {};
    }
    
    Result onExplainDoWhileBlock(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxWhileBlock* block = new SyntaxWhileBlock(context);
        block->exp = (SyntaxExp*)es[4].ptr;
        block->block = (SyntaxBlock*)es[1].ptr;
        block->type = SyntaxWhileBlock::DOWHILE;
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