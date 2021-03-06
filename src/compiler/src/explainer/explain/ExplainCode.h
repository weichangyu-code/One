#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxSentence.h"
#include "../syntax/SyntaxElement.h"
#include "../syntax/SyntaxBlock.h"
#include "../syntax/SyntaxExp.h"
#include "../syntax/SyntaxInstruct.h"
#include "../syntax/SyntaxIfBlock.h"
#include "../syntax/SyntaxWhileBlock.h"
#include "../syntax/SyntaxForBlock.h"

class ExplainCode : public ExplainBase
{
public:
    ExplainCode(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("element", "sentence", (MyRuleExecuteFunction)&onExplainElementSentence);
        registe("element", "codeblock", (MyRuleExecuteFunction)&onExplainElementCodeBlock);
        registe("element", "ifblock", (MyRuleExecuteFunction)&onExplainElementIfBlock);
        registe("element", "whileblock", (MyRuleExecuteFunction)&onExplainElementWhileBlock);
        registe("element", "dowhileblock", (MyRuleExecuteFunction)&onExplainElementWhileBlock);
        registe("element", "forblock", (MyRuleExecuteFunction)&onExplainElementForBlock);
        
        registe("elements", "", (MyRuleExecuteFunction)&onExplainElements);
        registe("elements", "add", (MyRuleExecuteFunction)&onExplainElementsAdd);

        registe("codeblock", "", (MyRuleExecuteFunction)&onExplainCodeBlock);
        registe("codeblock", "null", (MyRuleExecuteFunction)&onExplainCodeBlockNull);
    }

    Result onExplainElementSentence(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxSentence* sentence = (SyntaxSentence*)es[0].ptr;

        SyntaxElement* element = new SyntaxElement(context);
        element->type = SyntaxElement::SENTENCE;
        element->sentence = sentence;
        out.ptr = element;
        return {};
    }

    Result onExplainElementCodeBlock(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxBlock* block = (SyntaxBlock*)es[0].ptr;

        SyntaxElement* element = new SyntaxElement(context);
        element->type = SyntaxElement::CODEBLOCK;
        element->block = block;
        out.ptr = element;
        return {};
    }

    Result onExplainElementIfBlock(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxIfBlock* block = (SyntaxIfBlock*)es[0].ptr;

        SyntaxElement* element = new SyntaxElement(context);
        element->type = SyntaxElement::IFBLOCK;
        element->ifBlock = block;
        out.ptr = element;
        return {};
    }

    Result onExplainElementWhileBlock(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxWhileBlock* block = (SyntaxWhileBlock*)es[0].ptr;

        SyntaxElement* element = new SyntaxElement(context);
        element->type = SyntaxElement::WHILEBLOCK;
        element->whileBlock = block;
        out.ptr = element;
        return {};
    }

    Result onExplainElementForBlock(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxForBlock* block = (SyntaxForBlock*)es[0].ptr;

        SyntaxElement* element = new SyntaxElement(context);
        element->type = SyntaxElement::FORBLOCK;
        element->forBlock = block;
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

