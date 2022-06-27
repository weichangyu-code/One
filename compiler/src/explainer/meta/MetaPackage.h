#pragma once
#include "MetaBoxBase.h"

class MetaClass;
class MetaFile;
class MetaPackage : public MetaBoxBase
{
public:
    MetaPackage(const string& name, MetaPackage* outer, MetaContainer* metaContainer);
    
    MetaPackage* getPackage(const string& name, bool add = false);
    MetaClass* getClass(const string& name);
    MetaFile* getFile(const string& name);
    
    MetaPackage* addPackage(const string& name);
    MetaFile* addFile(const string& name, SyntaxBase* syntaxObj);
    void addClass(MetaClass* clazz);

public:
    string name;
    list<MetaPackage*> packages;
    list<MetaClass*> classes;
    list<MetaFile*> files;
};
