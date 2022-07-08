#include "MetaData.h"
#include "MetaType.h"
#include "MetaConst.h"
#include "MetaVarRef.h"
#include "MetaInstruct.h"
#include "MetaVariable.h"

MetaData::MetaData()
{

}

MetaData::MetaData(int type, void* ptr)
{
    this->type = type;
    this->const_ = (MetaConst*)ptr;
}
MetaData::MetaData(MetaConst* const_)
{
    this->type = CONST;
    this->const_ = const_;
}
MetaData::MetaData(MetaVarRef* varRef)
{
    this->type = VARREF;
    this->varRef = varRef;
}
MetaData::MetaData(MetaInstruct* instructTmp)
{;
    this->type = INSTRUCT;
    this->instructTmp = instructTmp;
}
MetaData::MetaData(MetaVariable* var)
{
    this->type = VAR;
    this->var = var;
}

void MetaData::setData(int type, void* ptr)
{
    this->type = type;
    this->const_ = (MetaConst*)ptr;
}
void MetaData::setData(MetaConst* const_)
{
    this->type = CONST;
    this->const_ = const_;
}
void MetaData::setData(MetaVarRef* varRef)
{
    this->type = VARREF;
    this->varRef = varRef;
}
void MetaData::setData(MetaInstruct* instructTmp)
{
    this->type = INSTRUCT;
    this->instructTmp = instructTmp;
}
void MetaData::setData(MetaVariable* var)
{
    this->type = VAR;
    this->var = var;
}
    
void MetaData::clear()
{
    this->type = NONE;
    this->const_ = nullptr;
}


bool MetaData::isNone()
{
    return this->type == NONE;
}

bool MetaData::isConst()
{
    return this->type == CONST;
}

bool MetaData::isVariable()
{
    return this->type == VARREF || this->type == VAR;
}
    
bool MetaData::isInstructTmp()
{
    return this->type == INSTRUCT;
}

MetaType MetaData::getType()
{
    if (this->type == CONST)
    {
        return const_->getType();
    }
    else if (this->type == VARREF)
    {
        return varRef->getType();
    }
    else if (this->type == INSTRUCT)
    {
        return instructTmp->retType;
    }
    else if (this->type == VAR)
    {
        return var->type;
    }
    else if (this->type == MEMBER)
    {
        return var->type;
    }
    return {};
}
    
bool MetaData::operator == (const MetaData& r)
{
    return this->type == r.type && this->const_ == r.const_;
}
