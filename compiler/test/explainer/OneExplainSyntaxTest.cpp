#include "StringUtils.cpp"
#include "SystemUtils.cpp"
#include "../../src/analyzer/rule/RuleAnalyzer.cpp"
#include "../../src/analyzer/lex/LexElement.cpp"
#include "../../src/analyzer/lex/LexAnalyzer.cpp"
#include "../../src/analyzer/map/RuleContainer.cpp"
#include "../../src/analyzer/map/RuleMap.cpp"
#include "../../src/analyzer/engine/RuleEngine.cpp"
#include "../../src/analyzer/engine/CodeAnalyzer.cpp"
#include "../../src/analyzer/one/OneRuleMap.cpp"
#include "../../src/analyzer/OneCodeAnalyzer.cpp"
#include "../../src/explainer/explain/ExplainContainer.cpp"
#include "../../src/explainer/explain/ExplainContext.cpp"
#include "../../src/explainer/syntax/SyntaxBase.cpp"
#include "../../src/explainer/syntax/SyntaxFile.cpp"
#include "../../src/explainer/syntax/SyntaxClass.cpp"
#include "../../common/src/FileUtils.cpp"

int main()
{
    ExplainContext explainContext;
    OneCodeAnalyzer oneCodeAnalyzer;
    ExplainContainer explainContainer(&oneCodeAnalyzer.ruleMap.ruleContainer, &explainContext);
    
    oneCodeAnalyzer.setRuleExecuteFuncion(std::bind(&ExplainContainer::callRuleExecuteFunction, &explainContainer
            , std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    explainContext.startSyntax();
    Result result = oneCodeAnalyzer.analysis(FileUtils::readFile("../../../one/test/helloone.one"));
    SyntaxFile* file = explainContext.finishSyntax();
    delete file;

    return 0;
}
