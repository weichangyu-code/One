#pragma once
#include "OnePackage.h"
#include "OneClass.h"
#include "OneField.h"
#include "OneFunction.h"

namespace One
{
    class IByteStream
    {
    public:
        IByteStream();

        void load(const void* data, unsigned int length);

        IByteStream& operator >> (string& str);
        IByteStream& operator >> (int& i);

    protected:
        void get(void* data, unsigned int len);

    protected:
        const char* _data = nullptr;
        unsigned int _readPos = 0;
        unsigned int _length = 0;
    };

    class MetaManager
    {
    public:
        MetaManager();

    public:
        void loadMeta(const void* data, unsigned int length);
        void loadObjectSize(const void* data, unsigned int length);
        void initClassP(Class*** arrayClassP);

        //
        template<class T>
        Reference<T> convertObjectType(const Reference<Object>& obj)
        {
            return Reference<T>((T*)convertType(obj.getObject(), ClassP<T>::getClass()), obj.isOwner(), true);
        }
        template<class T>
        Reference<T> convertInterfaceType(const Reference<Interface>& interface)
        {
            return Reference<T>((T*)convertType(interface.getObject()->getObject(), ClassP<T>::getClass()), interface.isOwner(), true);
        }

    protected:
        void loadPackage(Package* package);
        Class* loadClass();
        void loadFunction(Function* func);
        void loadField(Field* field);
        void loadParentOffset(Class* clazz, Class* parent);

        void* convertType(Object* obj, Class* type);
        
        Class* getClass(int id);
        Class* getClass(const string& classPath);

    protected:
        IByteStream _stream;

        Package* _root;
        vector<Class> _classes;
    };

    extern MetaManager g_metaManager;
}
