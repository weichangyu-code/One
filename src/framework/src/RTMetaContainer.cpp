#include "RTMetaContainer.h"
#include <string>
#include <assert.h>
using namespace std;

namespace One
{
    RTMetaContainer g_metaContainer;

    IByteStream::IByteStream()
    {
    }

    void IByteStream::load(const void* data, unsigned int length)
    {
        _data = (const char*)data;
        _length = length;
        _readPos = 0;
    }

    IByteStream& IByteStream::operator >> (string& str)
    {
        int length;
        *this >> length;
        str.resize(length);
        get((void*)str.data(), length);
        return *this;
    }
    IByteStream& IByteStream::operator >> (int& i)
    {
        get(&i, sizeof(i));
        return *this;
    }

    void IByteStream::get(void* data, unsigned int len)
    {
        if (_readPos + len > _length)
        {
            assert(_readPos + len <= _length);
        }
        memcpy(data, _data + _readPos, len);
        _readPos += len;
    }

    RTMetaContainer::RTMetaContainer()
    {

    }

    void RTMetaContainer::load(const void* data, unsigned int length)
    {
        _stream.load(data, length);

        int classNum;
        _stream >> classNum;
        _classes.resize(classNum + 1);
        for (int i = 1;i <= classNum;i++)
        {
            _classes[i] = new RTMetaClass();
            _classes[i]->id = i;
        }

        //这个后续优化，可以从私人缓存里面分配，提高速度，减少内存占用
        _root = new RTMetaPackage(nullptr);
        loadPackage(_root);
    }
        
    RTMetaClass* RTMetaContainer::getClass(int id)
    {
        return _classes[id];
    }
        
    void RTMetaContainer::loadPackage(RTMetaPackage* package)
    {
        _stream >> package->name;

        int subNum;
        _stream >> subNum;
        for (int i = 0;i < subNum;i++)
        {
            RTMetaPackage* subPackage = new RTMetaPackage(package);
            package->packages.push_back(subPackage);
            loadPackage(subPackage);
        }

        int classNum;
        _stream >> classNum;
        for (int i = 0;i < classNum;i++)
        {
            RTMetaClass* clazz = loadClass();
            clazz->package = package;
            package->classes.push_back(clazz);
        }
    }
        
    RTMetaClass* RTMetaContainer::loadClass()
    {
        int id;
        _stream >> id;
        RTMetaClass* clazz = getClass(id);

        _stream >> clazz->name;
        _classes[clazz->id] = clazz;

        int parentNum;
        _stream >> parentNum;
        clazz->parents.reserve(parentNum);
        for (int i = 0;i < parentNum;i++)
        {
            _stream >> id;
            clazz->parents.push_back(getClass(id));
        }

        int innerNum;
        _stream >> innerNum;
        clazz->inners.reserve(innerNum);
        for (int i = 0;i < innerNum;i++)
        {
            RTMetaClass* inner = loadClass();
            inner->outerClass = clazz;
            clazz->inners.push_back(inner);
        }

        int funcNum;
        _stream >> funcNum;
        clazz->funcs.reserve(funcNum);
        for (int i = 0;i < funcNum;i++)
        {
            RTMetaFunction* func = new RTMetaFunction(clazz);
            clazz->funcs.push_back(func);
            loadFunction(func);
        }

        int fieldNum;
        _stream >> fieldNum;
        clazz->fields.reserve(fieldNum);
        for (int i = 0;i < fieldNum;i++)
        {
            RTMetaField* field = new RTMetaField(clazz);
            clazz->fields.push_back(field);
            loadField(field);
        }

        return clazz;
    }
        
    void RTMetaContainer::loadFunction(RTMetaFunction* func)
    {
        _stream >> func->name;
    }
        
    void RTMetaContainer::loadField(RTMetaField* field)
    {
        _stream >> field->name;
    }
}
