
#include "ExplainBase.h"
#include "../syntax/SyntaxTryCatch.h"

class ExplainTryCatch : public ExplainBase
{
public:
    ExplainTryCatch(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("tryblock", "", (MyRuleExecuteFunction)&ExplainTryCatch::onExplainTryBlock);
        registe("catchblock", "var", (MyRuleExecuteFunction)&ExplainTryCatch::onExplainCatchBlockVar);
        registe("catchblock", "", (MyRuleExecuteFunction)&ExplainTryCatch::onExplainCatchBlock);
        //registe("finalblock", "", (MyRuleExecuteFunction)&ExplainTryCatch::onExplainFinalBlock);

        registe("trycatchstart", "", (MyRuleExecuteFunction)&ExplainTryCatch::onExplainTryCatchStart);
        registe("trycatchstart", "add", (MyRuleExecuteFunction)&ExplainTryCatch::onExplainTryCatchStartAdd);
        //registe("trycatch", "final", (MyRuleExecuteFunction)&ExplainTryCatch::onExplainTryCatchFinal);
        registe("trycatch", "", (MyRuleExecuteFunction)&ExplainTryCatch::onExplainTryCatch);

        registe("throw", "", (MyRuleExecuteFunction)&ExplainTryCatch::onExplainThrow);
    }
    
    Result onExplainTryBlock(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        out.ptr = es[1].ptr;
        return {};
    }
    
    Result onExplainCatchBlockVar(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxCatchBlock* catchBlock = new SyntaxCatchBlock(context);
        catchBlock->varDef = (SyntaxVarDef*)es[2].ptr;
        catchBlock->block = (SyntaxBlock*)es[4].ptr;

        out.ptr = catchBlock;
        return {};
    }
    
    Result onExplainCatchBlock(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxCatchBlock* catchBlock = new SyntaxCatchBlock(context);
        catchBlock->block = (SyntaxBlock*)es[1].ptr;

        out.ptr = catchBlock;
        return {};
    }
    
    // Result onExplainFinalBlock(Rule* rule, vector<LexElement>& es, LexElement& out)
    // {
    //     out.ptr = es[1].ptr;
    //     return {};
    // }
    
    Result onExplainTryCatchStart(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxTryCatch* tryCatchBlock = new SyntaxTryCatch(context);
        tryCatchBlock->tryBlock = (SyntaxBlock*)es[0].ptr;

        out.ptr = tryCatchBlock;
        return {};
    }
    
    Result onExplainTryCatchStartAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxTryCatch* tryCatchBlock = (SyntaxTryCatch*)es[0].ptr;
        tryCatchBlock->catchBlocks.push_back((SyntaxCatchBlock*)es[1].ptr);

        out.ptr = tryCatchBlock;
        return {};
    }
    
    // Result onExplainTryCatchFinal(Rule* rule, vector<LexElement>& es, LexElement& out)
    // {
    //     SyntaxTryCatch* tryCatchBlock = (SyntaxTryCatch*)es[0].ptr;
    //     tryCatchBlock->finalBlock = (SyntaxBlock*)es[1].ptr;

    //     out.ptr = tryCatchBlock;
    //     return {};
    // }
    
    Result onExplainTryCatch(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        out.ptr = es[0].ptr;
        return {};
    }
    
    Result onExplainThrow(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxExp* exp = (SyntaxExp*)es[1].ptr;

        SyntaxInstruct* instruct = new SyntaxInstruct(context);
        instruct->cmd = THROW;
        instruct->params.push_back(exp->ret);
        exp->instructs.push_back(instruct);

        out.ptr = exp;
        return {};
    }
};
    