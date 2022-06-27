#include "../../src/analyzer/rule/RuleAnalyzer.cpp"
#include "../../src/analyzer/lex/LexElement.cpp"
#include "../../src/analyzer/lex/LexAnalyzer.cpp"
#include "../../src/analyzer/map/RuleContainer.cpp"
#include "../../src/analyzer/map/RuleMap.cpp"
#include "../../src/analyzer/engine/RuleEngine.cpp"
#include "../../src/analyzer/engine/CodeAnalyzer.cpp"
#include "../../common/src/StringUtils.cpp"
#include "../../common/src/SystemUtils.cpp"
#include "../../common/src/FileUtils.cpp"

int main()
{
    unsigned int startTick = SystemUtils::getMSTick();
    
    RuleMap ruleMap;
    ruleMap.init(FileUtils::readFile("../../src/analyzer/rule/one.rule"));

    printf("RuleMap tick=%d\n", SystemUtils::getMSTick() - startTick);
    startTick = SystemUtils::getMSTick();

    CodeAnalyzer codeAnalyzer(&ruleMap.block);
    list<LexElement> elements = codeAnalyzer.analysis(FileUtils::readFile("../../../one/test/helloone.one"));
    //list<LexElement> elements = codeAnalyzer.analysis("class ff { int i = a + b * c + d - e; }");

    RuleEngine ruleEngine(ruleMap);
    auto result = ruleEngine.execute(elements);

    printf("RuleEngine %s tick=%d\n", result.isSuccess() ? "success" : "failed", SystemUtils::getMSTick() - startTick);

    return 0;
}
