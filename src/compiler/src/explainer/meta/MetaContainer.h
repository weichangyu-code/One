#pragma once
#include "MetaBoxBase.h"
#include "../../common/Result.h"
#include "MetaData.h"

class MetaType;
class MetaPackage;
class MetaVarRef;
class MetaContainer
{
public:
    MetaContainer();
    ~MetaContainer();

//包类管理
public:
	MetaPackage* getPackage(const string& path, bool add = false);
	MetaPackage* getPackage(const list<string>& path, bool add = false);
    MetaPackage* getRootPackage();

    MetaClass*   getClass(const string& path);
    MetaClass*   getStringClass();
    MetaClass*   getObjectClass();
    MetaClass*   getArrayClass();
    bool         isArray(const MetaType& type);

    list<MetaClass*>& getClasses();

protected:
    MetaPackage* rootPackage = nullptr;
    list<MetaClass*> classes;
    MetaClass* stringClass = nullptr;
    MetaClass* objectClass = nullptr;
    MetaClass* arrayClass = nullptr;

//对象管理
public:
    void addMetaObject(MetaBase* metaObj);
protected:
    list<MetaBase*> metaObjs;

//搜索
public:
    //查找内嵌类、同包类、Import类
    MetaClass* searchClass(MetaBoxBase* box, const string& name);

    //查找模板参数
    MetaTemplateParam* searchTemplateParam(MetaBoxBase* box, const string& name);

    //搜索import包、本包、顶级包
    MetaPackage* searchPackage(MetaBoxBase* box, const string& name);

    //查找本来和父类最匹配的方法
    MetaFunc* searchFunction(MetaClass* clazz, const string& name, list<MetaData>& params, bool onlyStatic);

    //从本box往上找，直到Class为止。如果是内嵌类，上层类的变量无法直接访问的
    MetaVarRef* searchVariable(MetaBoxBase* box, const string& name, bool onlyStatic);

    //是否支持自动转换
    bool canAutoConvertType(MetaType& src, MetaType& dst);

protected:
    MetaFunc* searchMatchFunction(MetaClass* clazz, const string& name, list<MetaData>& params, int& matchValue, bool onlyStatic);

};