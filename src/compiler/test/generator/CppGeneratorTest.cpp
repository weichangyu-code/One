// #include "StringUtils.cpp"
// #include "SystemUtils.cpp"
// #include "FileUtils.cpp"
// #include "../../src/analyzer/rule/RuleAnalyzer.cpp"
// #include "../../src/analyzer/lex/LexElement.cpp"
// #include "../../src/analyzer/lex/LexAnalyzer.cpp"
// #include "../../src/analyzer/map/RuleContainer.cpp"
// #include "../../src/analyzer/map/RuleMap.cpp"
// #include "../../src/analyzer/engine/RuleEngine.cpp"
// #include "../../src/analyzer/engine/CodeAnalyzer.cpp"
// #include "../../src/analyzer/one/OneRuleMap.cpp"
// #include "../../src/analyzer/OneCodeAnalyzer.cpp"
// #include "../../src/explainer/explain/ExplainContainer.cpp"
// #include "../../src/explainer/explain/ExplainContext.cpp"
// #include "../../src/explainer/syntax/SyntaxBase.cpp"
// #include "../../src/explainer/syntax/SyntaxFile.cpp"
// #include "../../src/explainer/syntax/SyntaxClass.cpp"
// #include "../../src/explainer/OneExplainer.cpp"
// #include "../../src/explainer/meta/MetaContainer.cpp"
// #include "../../src/explainer/meta/MetaBase.cpp"
// #include "../../src/explainer/meta/MetaBoxBase.cpp"
// #include "../../src/explainer/meta/MetaPackage.cpp"
// #include "../../src/explainer/meta/MetaBlock.cpp"
// #include "../../src/explainer/meta/MetaClass.cpp"
// #include "../../src/explainer/meta/MetaData.cpp"
// #include "../../src/explainer/meta/MetaFile.cpp"
// #include "../../src/explainer/meta/MetaFunc.cpp"
// #include "../../src/explainer/meta/MetaGenerator.cpp"
// #include "../../src/explainer/meta/MetaType.cpp"
// #include "../../src/explainer/meta/MetaVarRef.cpp"
// #include "../../src/explainer/meta/MetaVariable.cpp"
// #include "../../src/generator/CppGenerator.cpp"

#include "../../src/explainer/OneExplainer.h"
#include "../../src/generator/CppGenerator.h"

//这个测试废弃
int main()
{
    string cwd = "";

    OneExplainer oneExplainer;
    auto result = oneExplainer.explain("../../../one", "one");
    result = oneExplainer.generate();

    CppGenerator cppGenerator(oneExplainer.getMetaContainer());
    //cppGenerator.generate("HelloOne", "one.test.HelloOne", "../../../", "../onetest");

    return 0;
}
