#include "MetaBoxBase.h"

MetaBoxBase::MetaBoxBase(int boxType, MetaBoxBase* outer, MetaContainer* metaContainer, SyntaxBase* syntaxObj)
    :MetaBase(metaContainer, syntaxObj)
{
    this->boxType = boxType;
    this->outer = outer;
}

MetaPackage* MetaBoxBase::convertPackage()
{
    if (boxType == BOX_PACKAGE)
    {
        return (MetaPackage*)this;
    }
    else
    {
        return nullptr;
    }
}

MetaClass* MetaBoxBase::convertClass()
{
    if (boxType == BOX_CLASS)
    {
        return (MetaClass*)this;
    }
    else
    {
        return nullptr;
    }
}

MetaFunc* MetaBoxBase::convertFunc()
{
    if (boxType == BOX_FUNC)
    {
        return (MetaFunc*)this;
    }
    else
    {
        return nullptr;
    }
}

MetaBlock* MetaBoxBase::convertBlock()
{
    if (boxType == BOX_BLOCK)
    {
        return (MetaBlock*)this;
    }
    else
    {
        return nullptr;
    }
}

MetaFile* MetaBoxBase::convertFile()
{
    if (boxType == BOX_FILE)
    {
        return (MetaFile*)this;
    }
    else
    {
        return nullptr;
    }
}
bool MetaBoxBase::isPackage()
{
    return boxType == BOX_PACKAGE;
}
bool MetaBoxBase::isClass()
{
    return boxType == BOX_CLASS;
}
bool MetaBoxBase::isFunc()
{
    return boxType == BOX_FUNC;
}
bool MetaBoxBase::isBlock()
{
    return boxType == BOX_BLOCK;
}
bool MetaBoxBase::isFile()
{
    return boxType == BOX_FILE;
}

MetaFile* MetaBoxBase::getOuterFile()
{
    MetaBoxBase* iter = this;
    while (iter)
    {
        if (iter->isFile())
        {
            return (MetaFile*)iter;
        }
        iter = iter->outer;
    }
    return nullptr;
}
    
MetaPackage* MetaBoxBase::getOuterPackage()
{
    MetaBoxBase* iter = this;
    while (iter)
    {
        if (iter->isPackage())
        {
            return (MetaPackage*)iter;
        }
        iter = iter->outer;
    }
    return nullptr;
}
    
MetaClass* MetaBoxBase::getOuterClass()
{
    MetaBoxBase* iter = this;
    while (iter)
    {
        if (iter->isClass())
        {
            return (MetaClass*)iter;
        }
        iter = iter->outer;
    }
    return nullptr;
}
    
MetaFunc* MetaBoxBase::getOuterFunc()
{
    MetaBoxBase* iter = this;
    while (iter)
    {
        if (iter->isFunc())
        {
            return (MetaFunc*)iter;
        }
        iter = iter->outer;
    }
    return nullptr;
}
    
MetaVariable* MetaBoxBase::getVariable(const string& name, int filterType)
{
    return nullptr;
}
