#pragma once

namespace One
{
    class Object;
    class Interface
    {
    public:
        Interface(Object* obj);
        virtual ~Interface();

    public:
        void __acquireObj__(bool inner);
        void __releaseObj__(bool inner);
        void __destroyObj__();

    protected:
        Object* __obj__;
    };
} // namespace One

