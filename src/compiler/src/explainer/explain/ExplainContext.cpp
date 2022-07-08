#include "ExplainContext.h"
#include "../syntax/SyntaxBase.h"
#include "../syntax/SyntaxFile.h"


ExplainContext::ExplainContext()
{
}

ExplainContext::ExplainContext(SyntaxFile* file)
{
    this->file = file;
}

ExplainContext::~ExplainContext()
{
    
}

void ExplainContext::startSyntax()
{
    file = new SyntaxFile(this);
}

SyntaxFile* ExplainContext::finishSyntax()
{
    SyntaxFile* ret = file;
    file = nullptr;
    return ret;
}

void ExplainContext::addSyntaxObject(SyntaxBase* obj)
{
    file->addSyntaxObject(obj);
}
