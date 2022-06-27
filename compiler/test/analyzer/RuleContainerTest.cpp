#include "../../src/analyzer/rule/RuleAnalyzer.cpp"
#include "../../src/analyzer/lex/LexElement.cpp"
#include "../../src/analyzer/lex/LexAnalyzer.cpp"
#include "../../src/analyzer/map/RuleContainer.cpp"
#include "../../common/src/StringUtils.cpp"
#include "../../common/src/FileUtils.cpp"

int main()
{
    RuleAnalyzer ruleAnalyzer;
    RuleBlock block;
    ruleAnalyzer.parse(FileUtils::readFile("../../src/analyzer/rule/one.rule"), &block);

    RuleContainer ruleContainer;
    ruleContainer.init(&block);

    return 0;
}
