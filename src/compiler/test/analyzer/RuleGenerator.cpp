// #include "../../src/analyzer/rule/RuleAnalyzer.cpp"
// #include "../../src/analyzer/lex/LexElement.cpp"
// #include "../../src/analyzer/lex/LexAnalyzer.cpp"
// #include "../../src/analyzer/map/RuleContainer.cpp"
// #include "../../src/analyzer/map/RuleMap.cpp"
// #include "../../src/analyzer/one/OneRuleMap.cpp"
// #include "../../common/src/StringUtils.cpp"
// #include "../../common/src/FileUtils.cpp"
// #include "../../common/src/SystemUtils.cpp"

#include "../../src/analyzer/one/OneRuleMap.h"
#include "FileUtils.h"

int main()
{
    OneRuleMap ruleMap;
    ruleMap.init(FileUtils::readFile("../../src/analyzer/rule/one.rule"));
    ruleMap.generateInitOne("../../src/analyzer/one/OneRuleMap.inl");

    return 0;
}
