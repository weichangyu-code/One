
#include "ExplainBase.h"
#include "../syntax/SyntaxSentence.h"
#include "../syntax/SyntaxElement.h"
#include "../syntax/SyntaxBlock.h"
#include "../syntax/SyntaxExp.h"
#include "../syntax/SyntaxInstruct.h"
#include "../syntax/SyntaxVarDef.h"

class ExplainSentence : public ExplainBase
{
public:
    ExplainSentence(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("shortsentence", "exp", (MyRuleExecuteFunction)&onExplainShortSentenceExp);
        registe("shortsentence", "vardef", (MyRuleExecuteFunction)&onExplainShortSentenceVarDef);
        registe("multishortsentence", "", (MyRuleExecuteFunction)&onExplainMultiShortSentence);
        registe("multishortsentence", "add", (MyRuleExecuteFunction)&onExplainMultiShortSentenceAdd);

        registe("sentence", "return", (MyRuleExecuteFunction)&onExplainSentenceReturn);
        registe("sentence", "break", (MyRuleExecuteFunction)&onExplainSentenceBreak);
        registe("sentence", "continue", (MyRuleExecuteFunction)&onExplainSentenceContinue);
        registe("sentence", "short", (MyRuleExecuteFunction)&onExplainSentenceShort);
        registe("sentence", "end", (MyRuleExecuteFunction)&onExplainSentenceEnd);
    }
    
    Result onExplainShortSentenceExp(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return {};
    }
    
    Result onExplainShortSentenceVarDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVarDef* varDef = (SyntaxVarDef*)es[0].ptr;
        out.ptr = varDef->exp;
        return {};
    }
    
    Result onExplainMultiShortSentence(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return {};
    }

    Result onExplainMultiShortSentenceAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
        SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

        exp->append(exp2);
        exp->ret = exp2->ret;

        out.ptr = exp;
        return {};
    }

    Result onExplainSentenceReturn(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[0].ptr;

        SyntaxSentence* sentence = new SyntaxSentence(context);
        sentence->exp = exp;

        out.ptr = sentence;
        return {};
    }

    Result onExplainSentenceBreak(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[0].ptr;

        SyntaxSentence* sentence = new SyntaxSentence(context);
        sentence->exp = exp;

        out.ptr = sentence;
        return {};
    }

    Result onExplainSentenceContinue(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[0].ptr;

        SyntaxSentence* sentence = new SyntaxSentence(context);
        sentence->exp = exp;

        out.ptr = sentence;
        return {};
    }

    Result onExplainSentenceShort(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[0].ptr;

        SyntaxSentence* sentence = new SyntaxSentence(context);
        sentence->exp = exp;

        out.ptr = sentence;
        return {};
    }

    Result onExplainSentenceEnd(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxSentence* sentence = new SyntaxSentence(context);

        //空语句
        sentence->exp = new SyntaxExp(context);

        out.ptr = sentence;
        return {};
    }

};