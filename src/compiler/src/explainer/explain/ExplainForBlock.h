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
    
    Result onExplainForStart(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxForBlock* block = new SyntaxForBlock(context);
        block->exp[0] = (SyntaxExp*)es[2].ptr;
        block->exp[1] = (SyntaxExp*)es[4].ptr;
        block->exp[2] = (SyntaxExp*)es[6].ptr;
        block->type = SyntaxForBlock::FOR;
        out.ptr = block;
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