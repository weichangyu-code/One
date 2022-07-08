#include "SyntaxFile.h"


SyntaxFile::SyntaxFile(ExplainContext* context)
    :SyntaxBase(nullptr)
{

}

SyntaxFile::~SyntaxFile()
{
    for (auto& obj : syntaxObjs)
    {
        delete obj;
    }
}

void SyntaxFile::addSyntaxObject(SyntaxBase* obj)
{
    syntaxObjs.push_back(obj);
}