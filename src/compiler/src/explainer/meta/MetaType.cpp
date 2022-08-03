#include "MetaType.h"


MetaType::MetaType(int type)
{
    this->type = type;
}
MetaType::MetaType(MetaClass* clazz)
{
    this->type = DT_CLASS;
    this->clazz = clazz;
}

void MetaType::setClass(MetaClass* clazz)
{
    this->type = DT_CLASS;
    this->clazz = clazz;
}

void MetaType::setType(int type)
{
    this->type = type;
    this->clazz = nullptr;
}
    
void MetaType::setBool()
{
    setType(DT_BOOL);
}
    
void MetaType::setNull()
{
    setType(DT_NULL);
}

//pair<int, string>
struct ONE_BASE_TYPE
{
    int type;
    const char* oneType;
    const char* cppType;
};
ONE_BASE_TYPE ONE_BASE_TYPES[] = {
    {DT_VOID , "void", "void"},
    {DT_BOOL , "bool", "bool"},
    {DT_CHAR , "char", "signed char"},
    {DT_SHORT , "short", "signed short"},
    {DT_INT , "int", "signed int"},
    {DT_LONG , "long", "signed long long"},
    {DT_FLOAT , "float", "float"},
    {DT_DOUBLE , "double", "double"}
};
    
int MetaType::stringToType(const string& type)
{
    for (auto& t : ONE_BASE_TYPES)
    {
        if (type == t.oneType)
        {
            return t.type;
        }
    }
    return DT_NONE;
}
    
string MetaType::typeToString(int type)
{
    for (auto& t : ONE_BASE_TYPES)
    {
        if (t.type == type)
        {
            return t.oneType;
        }
    }
    return "";
}
    
string MetaType::typeToCppString(int type)
{
    for (auto& t : ONE_BASE_TYPES)
    {
        if (t.type == type)
        {
            return t.cppType;
        }
    }
    return "";
}

void MetaType::setType(const string& type)
{
    setType(stringToType(type));
}
	
bool MetaType::isNone() const
{
    return this->type == DT_NONE;
}

bool MetaType::isClass() const
{
    return this->type == DT_CLASS;
}
    
bool MetaType::isBaseType() const
{
    return this->type != DT_CLASS;
}

bool MetaType::isInteger() const
{
    switch (type)
    {
    case DT_CHAR:
    case DT_SHORT:
    case DT_INT:
    case DT_LONG:
        return true;
    }
    return false;
}
    
bool MetaType::isFloat() const
{
    switch (type)
    {
    case DT_FLOAT:
    case DT_DOUBLE:
        return true;
    }
    return false;
}

bool MetaType::isRealNumber() const
{
    return isInteger() || isFloat();
}
    
bool MetaType::isBool() const
{
    return type == DT_BOOL;
}
    
bool MetaType::isVoid() const
{
    return this->type == DT_VOID;
}
    
bool MetaType::isNull() const
{
    return this->type == DT_NULL;
}

bool MetaType::equal(const MetaType& r) const
{
    return this->type == r.type && this->clazz == r.clazz;
}

int MetaType::compare(const MetaType& r) const
{
    if (this->type != r.type)
    {
        return this->type - r.type;
    }
    if (this->type == DT_CLASS)
    {
        if (this->clazz != r.clazz)
        {
            return (this->clazz > r.clazz) ? 1 : -1;
        }
    }
    return 0;
}

bool MetaType::operator < (const MetaType& r) const
{
    return compare(r) < 0;
}
    
bool MetaType::operator == (const MetaType& r) const
{
    return equal(r);
}
    
bool MetaType::operator != (const MetaType& r) const
{
    return equal(r) == false;
}
    
MetaType MetaType::max(const MetaType& r1, const MetaType& r2)
{
    MetaType ret;
    ret.type = std::max(r1.type, r2.type);
    return ret;
}
