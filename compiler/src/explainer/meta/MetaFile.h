#pragma once
#include "MetaBoxBase.h"

class MetaPackage;
class MetaClass;
class MetaFile : public MetaBoxBase
{
public:
    MetaFile(const string& name, MetaPackage* package, MetaContainer* metaContainer, SyntaxBase* syntaxObj);
    virtual ~MetaFile();

    MetaClass* addClass(const string& name, SyntaxBase* syntaxObj);
    MetaClass* getClass(const string& name);

public:
    string name;
    list<MetaClass*> classes;

    list<MetaPackage*> importPackages;
    list<MetaClass*> importClasses;
};

