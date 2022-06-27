#include "../../src/analyzer/rule/RuleAnalyzer.cpp"
#include "../../src/analyzer/lex/LexElement.cpp"
#include "../../src/analyzer/lex/LexAnalyzer.cpp"
#include "../../src/analyzer/map/RuleContainer.cpp"
#include "../../src/analyzer/map/RuleMap.cpp"
#include "../../common/src/StringUtils.cpp"
#include "../../common/src/FileUtils.cpp"

int main()
{
    RuleMap ruleMap;
    ruleMap.init(FileUtils::readFile("../../src/analyzer/rule/one.rule"));
    return 0;
}
