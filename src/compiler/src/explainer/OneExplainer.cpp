#include "OneExplainer.h"
#include "FileUtils.h"
#include "meta/MetaClass.h"
#include "meta/MetaFile.h"
#include "syntax/SyntaxFile.h"

OneExplainer::OneExplainer()
    :explainContainer(&oneCodeAnalyzer.ruleMap.ruleContainer, &explainContext)
    ,metaGenerator(&metaContainer)
{
    //设置回调
    oneCodeAnalyzer.setRuleExecuteFuncion(std::bind(&ExplainContainer::callRuleExecuteFunction, &explainContainer
            , std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

}

Result OneExplainer::explain(const string& folder, const string& packagePath)
{
    MetaPackage* package = metaContainer.getPackage(packagePath, true);

    //语法分析
    VR(explainSyntaxFolder(folder, package));

    return {};
}
    
Result OneExplainer::generate()
{
    return metaGenerator.generate();
}
    
MetaContainer* OneExplainer::getMetaContainer()
{
    return &metaContainer;
}

Result OneExplainer::explainSyntax(const string& code, SyntaxFile** out)
{
    explainContext.startSyntax();
    Result result = oneCodeAnalyzer.analysis(code);
    *out = explainContext.finishSyntax();
    if (result.isError())
    {
        SAFE_DELETE(*out);
    }
    return result;
}

Result OneExplainer::explainSyntaxFolder(const string& folder, MetaPackage* package)
{
    list<string> subs = FileUtils::listSub(folder);
    for (auto& sub : subs)
    {
        string path = FileUtils::appendFileName(folder, sub);
        if (FileUtils::isDir(path))
        {
            VR(explainSyntaxFolder(path, package->getPackage(sub, true)));
        }
        else
        {
            if (FileUtils::getFileExt(sub) != "one")
            {
                continue;
            }
            printf("explain %s\n", sub.c_str());
            string code = FileUtils::readFile(path);
            if (code.empty())
            {
                continue;
            }
            SyntaxFile* syntaxFile = nullptr;
            VR(explainSyntax(code, &syntaxFile));
            if (syntaxFile->classes.size() != 1 || syntaxFile->classes.front()->name != FileUtils::getFileTitle(sub))
            {
                // 类里面超过一个类，或者类名不等于文件名，错误
                return R_FAILED;
            }
            VR(metaGenerator.addSyntaxFile(package, sub, syntaxFile));
        }
    }
    return {};
}
    

