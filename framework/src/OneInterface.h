#pragma once

namespace One
{
    class Object;
    class Interface
    {
    public:
        Interface(Object* obj);

    public:
        void acquireObj(bool inner);
        void releaseObj(bool inner);

    protected:
        Object* _obj;
    };
} // namespace One

