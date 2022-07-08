#include "MetaFile.h"
#include "../syntax/SyntaxFile.h"
#include "MetaClass.h"
#include "MetaPackage.h"

MetaFile::MetaFile(const string& name, MetaPackage* package, MetaContainer* metaContainer, SyntaxBase* syntaxObj)
    :MetaBoxBase(BOX_FILE, package, metaContainer, syntaxObj)
{
    this->name = name;
}
MetaFile::~MetaFile()
{
    delete (SyntaxFile*) syntaxObj;
}

MetaClass* MetaFile::addClass(const string& name, SyntaxBase* syntaxObj)
{
    MetaClass* clazz = new MetaClass(name, this, metaContainer, syntaxObj);
    classes.push_back(clazz);
    outer->convertPackage()->addClass(clazz);
    return clazz;
}

MetaClass* MetaFile::getClass(const string& name)
{
    for (auto& clazz : classes)
    {
        if (clazz->name == name)
        {
            return clazz;
        }
    }
    return nullptr;
}
