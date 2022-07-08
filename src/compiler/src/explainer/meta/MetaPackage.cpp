#include "MetaPackage.h"
#include "MetaClass.h"
#include "MetaFile.h"

MetaPackage::MetaPackage(const string& name, MetaPackage* outer, MetaContainer* metaContainer)
    :MetaBoxBase(BOX_PACKAGE, outer, metaContainer, nullptr)
{
    this->name = name;
}

MetaPackage* MetaPackage::getPackage(const string& name, bool add)
{
    for (auto& package : packages)
    {
        if (package->name == name)
        {
            return package;
        }
    }
    if (add)
    {
        return addPackage(name);
    }
    return nullptr;
}

MetaClass* MetaPackage::getClass(const string& name)
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

MetaFile* MetaPackage::getFile(const string& name)
{
    for (auto& file : files)
    {
        if (file->name == name)
        {
            return file;
        }
    }
    return nullptr;
}

MetaPackage* MetaPackage::addPackage(const string& name)
{
    MetaPackage* package = new MetaPackage(name, this, metaContainer);
    packages.push_back(package);
    return package;
}
    
void MetaPackage::addClass(MetaClass* clazz)
{
    classes.push_back(clazz);
}
    
MetaFile* MetaPackage::addFile(const string& name, SyntaxBase* syntaxObj)
{
    MetaFile* file = new MetaFile(name, this, metaContainer, syntaxObj);
    files.push_back(file);
    return file;
}
