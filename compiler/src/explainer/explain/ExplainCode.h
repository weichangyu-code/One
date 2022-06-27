#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxSentence.h"
#include "../syntax/SyntaxElement.h"
#include "../syntax/SyntaxBlock.h"
#include "../syntax/SyntaxExp.h"
#include "../syntax/SyntaxInstruct.h"

class ExplainCode : public ExplainBase
{
public:
    ExplainCode(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("element", "sentence", (MyRuleExecuteFunction)&onExplainElementSentence);
        registe("element", "block", (MyRuleExecuteFunction)&onExplainElementBlock);
        registe("element", "classdef", (MyRuleExecuteFunction)&onExplainElementClassDef);
        
        registe("elements", "", (MyRuleExecuteFunction)&onExplainElements);
        registe("elements", "add", (MyRuleExecuteFunction)&onExplainElementsAdd);

        registe("codeblock", "", (MyRuleExecuteFunction)&onExplainCodeBlock);
        registe("codeblock", "null", (MyRuleExecuteFunction)&onExplainCodeBlockNull);
    }

    Result onExplainElementSentence(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxSentence* sentence = (SyntaxSentence*)es[0].ptr;

        SyntaxElement* element = new SyntaxElement(context);
        element->sentence = sentence;
        out.ptr = element;
        return {};
    }

    Result onExplainElementBlock(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxBlock* block = (SyntaxBlock*)es[0].ptr;

        SyntaxElement* element = new SyntaxElement(context);
        element->block = block;
        out.ptr = element;
        return {};
    }

    Result onExplainElementClassDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxClass* clazz = (SyntaxClass*)es[0].ptr;

        SyntaxElement* element = new SyntaxElement(context);
        element->clazz = clazz;
        out.ptr = element;
        return {};
    }

    Result onExplainElements(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxElement* element = (SyntaxElement*)es[0].ptr;

        SyntaxBlock* block = new SyntaxBlock(context);
        block->elements.push_back(element);
        out.ptr = block;
        return {};
    }

    Result onExplainElementsAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxBlock* block = (SyntaxBlock*)es[0].ptr;
        SyntaxElement* element = (SyntaxElement*)es[1].ptr;

        block->elements.push_back(element);
        out.ptr = block;
        return {};
    }

    Result onExplainCodeBlock(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxBlock* block = (SyntaxBlock*)es[1].ptr;

        out.ptr = block;
        return {};
    }


    Result onExplainCodeBlockNull(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxBlock* block = new SyntaxBlock(context);

        out.ptr = block;
        return {};
    }

};

