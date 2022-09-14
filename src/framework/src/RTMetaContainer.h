#pragma once
#include "RTMetaPackage.h"
#include "RTMetaClass.h"
#include "RTMetaField.h"
#include "RTMetaFunction.h"

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

    class RTMetaContainer
    {
    public:
        RTMetaContainer();

    public:
        void load(const void* data, unsigned int length);

        RTMetaClass* getClass(int id);

    protected:
        void loadPackage(RTMetaPackage* package);
        RTMetaClass* loadClass();
        void loadFunction(RTMetaFunction* func);
        void loadField(RTMetaField* field);

    protected:
        IByteStream _stream;

        RTMetaPackage* _root;
        vector<RTMetaClass*> _classes;
    };

    extern RTMetaContainer g_metaContainer;
}
