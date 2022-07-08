#pragma once
#include "SyntaxBase.h"
#include "SyntaxClass.h"
#include "SyntaxMulti.h"

class SyntaxFile : public SyntaxBase
{
public:
    SyntaxFile(ExplainContext* context);
    virtual ~SyntaxFile();

//临时对象管理
public:
    void addSyntaxObject(SyntaxBase* obj);
protected:
    list<SyntaxBase*> syntaxObjs;

//
public:
    list<SyntaxClass*> classes;
    list<SyntaxMulti<string>*> imports;
};
