#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxClass.h"
#include "../syntax/SyntaxFile.h"
#include "../syntax/SyntaxMulti.h"

class ExplainFile : public ExplainBase
{
public:
    ExplainFile(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("fileelement", "classdef", (MyRuleExecuteFunction)&ExplainFile::onExplainClassDef);

        registe("import", "", (MyRuleExecuteFunction)&ExplainFile::onExplainImport);
    }

    Result onExplainClassDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxClass* clazz = (SyntaxClass*)es[0].ptr;
        context->file->classes.push_back(clazz);
        return {};
    }

    Result onExplainImport(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<string>* idpath = (SyntaxMulti<string>*)es[1].ptr;
        context->file->imports.push_back(idpath);
        return {};
    }
};
