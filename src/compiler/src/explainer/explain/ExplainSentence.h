
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
        registe("shortsentence", "exp", (MyRuleExecuteFunction)&ExplainSentence::onExplainShortSentenceExp);
        registe("shortsentence", "vardef", (MyRuleExecuteFunction)&ExplainSentence::onExplainShortSentenceVarDef);
        registe("multishortsentence", "", (MyRuleExecuteFunction)&ExplainSentence::onExplainMultiShortSentence);
        registe("multishortsentence", "add", (MyRuleExecuteFunction)&ExplainSentence::onExplainMultiShortSentenceAdd);

        registe("sentence", "return", (MyRuleExecuteFunction)&ExplainSentence::onExplainSentenceReturn);
        registe("sentence", "break", (MyRuleExecuteFunction)&ExplainSentence::onExplainSentenceBreak);
        registe("sentence", "continue", (MyRuleExecuteFunction)&ExplainSentence::onExplainSentenceContinue);
        registe("sentence", "short", (MyRuleExecuteFunction)&ExplainSentence::onExplainSentenceShort);
        registe("sentence", "end", (MyRuleExecuteFunction)&ExplainSentence::onExplainSentenceEnd);
    }
    
    Result onExplainShortSentenceExp(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return {};
    }
    
    Result onExplainShortSentenceVarDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxVarDef* varDef = (SyntaxVarDef*)es[0].ptr;
        varDef->addVarDefAndAssginInstruct(context);
        out.ptr = varDef->exp;
        return {};
    }
    
    Result onExplainMultiShortSentence(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
		SyntaxExp* exp = (SyntaxExp*)es[0].ptr;

		SyntaxMulti<SyntaxExp*>* multiExp = new SyntaxMulti<SyntaxExp*>(context);
		multiExp->items.push_back(exp);

		out.ptr = multiExp;
		return {};
    }

    Result onExplainMultiShortSentenceAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        // SyntaxExp* exp = (SyntaxExp*)es[0].ptr;
        // SyntaxExp* exp2 = (SyntaxExp*)es[2].ptr;

        // SyntaxInstruct* instruct = exp->instructs.back();
        // if (instruct->cmd != COMMA)
        // {
        //     instruct = new SyntaxInstruct(context);
        //     instruct->cmd = COMMA;
        // }
        // else
        // {
        //     exp->instructs.pop_back();
        // }

        // instruct->params.push_back(exp2->ret);
        // exp->append(exp2);

        // exp->instructs.push_back(instruct);
        // exp->ret.setInstruct(instruct);

        // out.ptr = exp;
        // return {};
        
		SyntaxMulti<SyntaxExp*>* multiExp = (SyntaxMulti<SyntaxExp*>*)es[0].ptr;
		SyntaxExp* exp = (SyntaxExp*)es[2].ptr;

		multiExp->items.push_back(exp);

		out.ptr = multiExp;
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
        SyntaxMulti<SyntaxExp*>* multiExp = (SyntaxMulti<SyntaxExp*>*)es[0].ptr;
        
        SyntaxSentence* sentence = new SyntaxSentence(context);
        sentence->exp = SyntaxExp::combine(multiExp, false, context);

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