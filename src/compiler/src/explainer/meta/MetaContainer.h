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
    MetaClass*   getInterfaceClass();
    MetaClass*   getArrayClass();
    MetaClass*   getIterableClass();
    MetaClass*   getClassClass();
    bool         isArray(const MetaType& type);

    void         addClass(MetaClass* clazz);
    const list<MetaClass*>& getClasses();

    MetaConst*   getZeroConst();

protected:
    MetaPackage* rootPackage = nullptr;
    list<MetaClass*> classes;
    MetaClass* stringClass = nullptr;
    MetaClass* objectClass = nullptr;
    MetaClass* interfaceClass = nullptr;
    MetaClass* arrayClass = nullptr;
    MetaClass* iterableClass = nullptr;
    MetaClass* classClass = nullptr;

    MetaConst* zeroConst = nullptr;

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
    MetaFunc* searchClassFunction(MetaClass* clazz, const string& name, const list<MetaData>& params, int filterType);

    //从Box开始查找Function
    MetaFunc* searchFunction(MetaBoxBase* box, const string& name, const list<MetaData>& params, int filterType, MetaVarRef** varRef);

    //从本box往上找，直到Class为止。如果是内嵌类，上层类的变量无法直接访问的
    MetaVarRef* searchVariable(MetaBoxBase* box, const string& name, int filterType);

protected:
    MetaFunc* searchMatchClassFunction(MetaClass* clazz, const string& name, const list<MetaData>& params, int& matchValue, int filterType);

//类型转换
//自动转换类型：
//0. 类型一样
//1. 基础类型的转换
//2. 子类到父类的转换，需要代码
//3. 通过构造函数的转换，需要新建对象
public:
    enum
    {
        ACT_CANNT = -1,
        ACT_EQUAL = 0,          //类型一样，不需要转换
        ACT_BASE_TYPE,          //基础类型自动转换，大的转换成小的
        ACT_NULL,               //null转换成任何类型
        ACT_PARENT_TYPE,        //子类到父类，需要叠加代码
        ACT_TEMPLATE,           //模板参数继承
        ACT_CONSTRUCT,          //通过构造函数转换
        ACT_VALUEOF,            //通过valueOf函数转换
    };
    void addAutoConvertType(const MetaType& src, const MetaType& dst, int type);
    int  getAutoConvertType(const MetaType& src, const MetaType& dst);

protected:
    void addDefaultAutoConvert();

protected:
    map<MetaType, map<MetaType, int> > autoConvertData;
};