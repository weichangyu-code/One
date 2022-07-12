#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxIfBlock.h"
#include "Common.h"

class ExplainIfBlock : public ExplainBase
{
public:
    ExplainIfBlock(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("ifstart", "", (MyRuleExecuteFunction)&onExplainIfStart);
        registe("if", "", (MyRuleExecuteFunction)&onExplainIf);
        registe("else", "", (MyRuleExecuteFunction)&onExplainElse);
        registe("elseifstart", "", (MyRuleExecuteFunction)&onExplainElseIfStart);
        registe("elseif", "", (MyRuleExecuteFunction)&onExplainElseIf);
        registe("ifblockstart", "", (MyRuleExecuteFunction)&onExplainIfBlockStart);
        registe("ifblockstart", "elseif", (MyRuleExecuteFunction)&onExplainIfBlockStartElseIf);
        registe("ifblock", "else", (MyRuleExecuteFunction)&onExplainIfBlockElse);
        //registe("ifblock", "", (MyRuleExecuteFunction)&onExplainIfBlock);
    }
    
    Result onExplainIfStart(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxIfItem* ifItem = new SyntaxIfItem(context);
        ifItem->exp = (SyntaxExp*)es[2].ptr;
        out.ptr = ifItem;
        return {};
    }
    
    Result onExplainIf(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxIfItem* ifItem = (SyntaxIfItem*)es[0].ptr;
        ifItem->block = (SyntaxBlock*)es[1].ptr;
        out.ptr = ifItem;
        return {};
    }
    
    Result onExplainElse(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxIfItem* ifItem = new SyntaxIfItem(context);
        ifItem->block = (SyntaxBlock*)es[1].ptr;
        out.ptr = ifItem;
        return {};
    }
    
    Result onExplainElseIfStart(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxIfItem* ifItem = new SyntaxIfItem(context);
        ifItem->exp = (SyntaxExp*)es[3].ptr;
        out.ptr = ifItem;
        return {};
    }
    
    Result onExplainElseIf(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxIfItem* ifItem = (SyntaxIfItem*)es[0].ptr;
        ifItem->block = (SyntaxBlock*)es[1].ptr;
        out.ptr = ifItem;
        return {};
    }
    
    Result onExplainIfBlockStart(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxIfBlock* ifBlock = new SyntaxIfBlock(context);
        ifBlock->items.push_back((SyntaxIfItem*)es[0].ptr);
        out.ptr = ifBlock;
        return {};
    }
    
    Result onExplainIfBlockStartElseIf(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxIfBlock* ifBlock = (SyntaxIfBlock*)es[0].ptr;
        ifBlock->items.push_back((SyntaxIfItem*)es[1].ptr);
        out.ptr = ifBlock;
        return {};
    }
    
    Result onExplainIfBlockElse(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxIfBlock* ifBlock = (SyntaxIfBlock*)es[0].ptr;
        ifBlock->items.push_back((SyntaxIfItem*)es[1].ptr);
        out.ptr = ifBlock;
        return {};
    }
};