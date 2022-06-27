#pragma once
#include <stdint.h>
#include <bits/move.h>

namespace One
{
    template<typename T>
    class Pointer
    {
    public:
        Pointer()
        {}
        Pointer(T* ptr, bool inner)
        {
            set(ptr, inner);
        }
        Pointer(const Pointer<T>& r)
        {
            set(r);
        }

        void set(T* ptr, bool inner)
        {
            if (ptr)
            {
                this->ptr = (char*)ptr + (inner ? 1 : 0);
            }
            else
            {
                this->ptr = nullptr;
            }
        }
        void set(const Pointer<T>& r)
        {
            this->ptr = r.ptr;
        }
        void clear()
        {
            this->ptr = nullptr;
        }

        T* getObject() const
        {
            register char* ptr2 = (char*)ptr;
            return (T*) ((intptr_t)ptr2 & (~0x3));
        }
        bool isInner() const
        {
            return ((intptr_t)ptr & 0x1) == 1;
        }
        bool isNull() const
        {
            return ptr == nullptr;
        }

        bool operator==(const Pointer<T>& r) const
        {
            return getObject() == r.getObject();
        }
        bool operator!=(const Pointer<T>& r) const
        {
            return getObject() != r.getObject();
        }
        bool operator>(const Pointer<T>& r) const
        {
            return getObject() > r.getObject();
        }
        bool operator<(const Pointer<T>& r) const
        {
            return getObject() < r.getObject();
        }

        Pointer<T>& operator = (const Pointer<T>& r)
        {
            set(r);
            return *this;
        }
        Pointer<T>& operator = (T* ptr)
        {
            set(ptr, false);
            return *this;
        }

        operator T*()
        {
            return getObject();
        }
        T* operator->()
        {
            return getObject();
        }
        operator bool()
        {
            return isNull() == false;
        }
        T& operator*()
        {
            return *getObject();
        }

        void acquire()
        {
            if (std::is_base_of<Object, T>::value)
            {
                Object* obj = (Object*)(void*)getObject();
                obj->acquire(isInner());
            }
            else
            {
                //规避多重继承的歧义问题
                Interface* i = (Interface*)(void*)getObject();
                i->acquireObj(isInner());
            }
        }
        
        void release()
        {
            if (std::is_base_of<Object, T>::value)
            {
                Object* obj = (Object*)(void*)getObject();
                obj->release(isInner());
            }
            else
            {
                //规避多重继承的歧义问题
                Interface* i = (Interface*)(void*)getObject();
                i->releaseObj(isInner());
            }
        }

    public:
        char* ptr = nullptr;
    };

}