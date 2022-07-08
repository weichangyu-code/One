#pragma once
#include "analyzer/OneCodeAnalyzer.h"
#include "explain/ExplainContainer.h"
#include "explain/ExplainContext.h"
#include "meta/MetaContainer.h"
#include "meta/MetaPackage.h"
#include "meta/MetaGenerator.h"

class OneExplainer
{
public:
    OneExplainer();

    Result explain(const string& folder, const string& packagePath);
    Result generate();
    
    MetaContainer* getMetaContainer();

protected:
    Result explainSyntax(const string& code, SyntaxFile** out);
    Result explainSyntaxFolder(const string& folder, MetaPackage* package);

protected:
    ExplainContext explainContext;
    OneCodeAnalyzer oneCodeAnalyzer;
    ExplainContainer explainContainer;

    MetaContainer metaContainer;
    MetaGenerator metaGenerator;
};
