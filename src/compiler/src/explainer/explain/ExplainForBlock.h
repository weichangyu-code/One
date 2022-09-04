#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxForBlock.h"
#include "Common.h"

class SyntaxVarDef;
class ExplainForBlock : public ExplainBase
{
public:
    ExplainForBlock(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("forstart", "range", (MyRuleExecuteFunction)&ExplainForBlock::onExplainForStartRange);
        registe("forstart", "each", (MyRuleExecuteFunction)&ExplainForBlock::onExplainForStartEach);
        registe("forstart0", "", (MyRuleExecuteFunction)&ExplainForBlock::onExplainForStart0);
        registe("forstart1", "", (MyRuleExecuteFunction)&ExplainForBlock::onExplainForStart1);
        registe("forstart1", "null", (MyRuleExecuteFunction)&ExplainForBlock::onExplainForStart1Null);
        registe("forstart2", "", (MyRuleExecuteFunction)&ExplainForBlock::onExplainForStart2);
        registe("forstart2", "null", (MyRuleExecuteFunction)&ExplainForBlock::onExplainForStart2Null);
        registe("forstart3", "", (MyRuleExecuteFunction)&ExplainForBlock::onExplainForStart3);
        registe("forstart3", "null", (MyRuleExecuteFunction)&ExplainForBlock::onExplainForStart3Null);
        registe("forstart", "", (MyRuleExecuteFunction)&ExplainForBlock::onExplainForStart);
        registe("forblock", "", (MyRuleExecuteFunction)&ExplainForBlock::onExplainFor);
    }
    
    Result onExplainForStartRange(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxForBlock* block = new SyntaxForBlock(context);
        block->varDef = (SyntaxVarDef*)es[2].ptr;
        block->exp[0] = (SyntaxExp*)es[5].ptr;
        block->exp[1] = (SyntaxExp*)es[7].ptr;
        block->type = SyntaxForBlock::FOR_RANGE;
        out.ptr = block;
        return {};
    }
    
    Result onExplainForStartEach(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxForBlock* block = new SyntaxForBlock(context);
        block->varDef = (SyntaxVarDef*)es[2].ptr;
        block->exp[0] = (SyntaxExp*)es[4].ptr;
        block->type = SyntaxForBlock::FOR_EACH;
        out.ptr = block;
        return {};
    }

    Result onExplainForStart0(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxForBlock* block = new SyntaxForBlock(context);
        block->type = SyntaxForBlock::FOR;
        out.ptr = block;
        return {};
    }

    Result onExplainForStart1(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxForBlock* block = (SyntaxForBlock*)es[0].ptr;
        block->exp[0] = (SyntaxExp*)es[1].ptr;
        out.ptr = block;
        return {};
    }

    Result onExplainForStart1Null(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return {};
    }

    Result onExplainForStart2(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxForBlock* block = (SyntaxForBlock*)es[0].ptr;
        block->exp[1] = (SyntaxExp*)es[1].ptr;
        out.ptr = block;
        return {};
    }

    Result onExplainForStart2Null(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return {};
    }

    Result onExplainForStart3(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxForBlock* block = (SyntaxForBlock*)es[0].ptr;
        block->exp[2] = (SyntaxExp*)es[1].ptr;
        out.ptr = block;
        return {};
    }

    Result onExplainForStart3Null(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return {};
    }
    
    Result onExplainForStart(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return {};
    }
    
    Result onExplainFor(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxForBlock* block = (SyntaxForBlock*)es[0].ptr;
        block->block = (SyntaxBlock*)es[1].ptr;
        out.ptr = block;
        return {};
    }
};