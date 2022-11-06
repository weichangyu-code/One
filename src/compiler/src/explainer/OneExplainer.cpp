#include "OneExplainer.h"
#include "FileUtils.h"
#include "FolderUtils.h"
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
    Result result;
    FolderUtils::findEach(folder, [&result, this, folder, package](const string& name, bool isDir) {
        string path = FileUtils::appendFileName(folder, name);
        if (isDir)
        {
            result = explainSyntaxFolder(path, package->getPackage(name, true));
            if (result.isError())
            {
                return false;
            }
        }
        else
        {
            if (FileUtils::getFileExt(name) != "one")
            {
                return true;
            }
            printf("explain %s\n", name.c_str());
            string code = FileUtils::readFile(path);
            if (code.empty())
            {
                return true;
            }
            SyntaxFile* syntaxFile = nullptr;
            result = explainSyntax(code, &syntaxFile);
            if (result.isError())
            {
                return false;
            }
            if (syntaxFile->classes.size() != 1 || syntaxFile->classes.front()->name != FileUtils::getFileTitle(name))
            {
                // 类里面超过一个类，或者类名不等于文件名，错误
                result = R_FAILED;
                return false;
            }
            result = metaGenerator.addSyntaxFile(package, name, syntaxFile);
            if (result.isError())
            {
                return false;
            }
        }
        return true;
    });
    return result;
}
    

