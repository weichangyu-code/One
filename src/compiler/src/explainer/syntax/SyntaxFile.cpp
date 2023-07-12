#include "SyntaxFile.h"
#include "../explain/ExplainContext.h"


SyntaxFile::SyntaxFile(ExplainContext* context)
    :SyntaxBase(nullptr)
{
    tempContext = new ExplainContext(this);
}

SyntaxFile::~SyntaxFile()
{
    for (auto& obj : syntaxObjs)
    {
        delete obj;
    }
    delete tempContext;
}

void SyntaxFile::addSyntaxObject(SyntaxBase* obj)
{
    syntaxObjs.push_back(obj);
}
    
ExplainContext* SyntaxFile::getTempExplainContext()
{
    return tempContext;
}