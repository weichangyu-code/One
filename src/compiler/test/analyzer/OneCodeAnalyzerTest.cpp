#include "../../src/analyzer/rule/RuleAnalyzer.cpp"
#include "../../src/analyzer/lex/LexElement.cpp"
#include "../../src/analyzer/lex/LexAnalyzer.cpp"
#include "../../src/analyzer/map/RuleContainer.cpp"
#include "../../src/analyzer/map/RuleMap.cpp"
#include "../../src/analyzer/engine/RuleEngine.cpp"
#include "../../src/analyzer/engine/CodeAnalyzer.cpp"
#include "../../src/analyzer/one/OneRuleMap.cpp"
#include "../../src/analyzer/OneCodeAnalyzer.cpp"
#include "../../common/src/StringUtils.cpp"
#include "../../common/src/SystemUtils.cpp"
#include "../../common/src/FileUtils.cpp"

Result executeRule(Rule* rule, vector<LexElement>& es, LexElement& out)
{
	printf("::executeRule name=%s alias=%s\n", rule->name.c_str(), rule->alias.c_str());
    return {};
}

int main()
{
    unsigned int startTick = SystemUtils::getMSTick();

    OneCodeAnalyzer oneCodeAnalyzer;
    oneCodeAnalyzer.setRuleExecuteFuncion(executeRule);

    printf("OneCodeAnalyzer tick=%d\n", SystemUtils::getMSTick() - startTick);
    startTick = SystemUtils::getMSTick();

    auto result = oneCodeAnalyzer.analysis(FileUtils::readFile("../../../one/test/helloone.one"));

    printf("analysis %s tick=%d\n", result.isSuccess() ? "success" : "failed", SystemUtils::getMSTick() - startTick);

    return 0;
}
