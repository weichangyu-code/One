#pragma once
//#include <stdint.h>
//#include <bits/move.h>
//#include <cstddef>

namespace One
{
    void throwNullPointExeption();

    //T可以是Object或者Interface，或者他们的子类
    template<typename T>
    class Pointer
    {
    public:
        Pointer()
        {}
        Pointer(nullptr_t n)
        {}
        Pointer(T* ptr)
        {
            if (ptr)
            {
                this->ptr = ptr;
            }
        }
        Pointer(T* ptr, bool owner)
        {
            if (ptr)
            {
                this->ptr = (char*)ptr + (owner ? 0 : 1);
            }
        }
        Pointer(const Pointer<T>& r)
        {
            this->ptr = r.ptr;
        }

        void set(T* ptr, bool owner)
        {
            if (ptr)
            {
                this->ptr = (char*)ptr + (owner ? 0 : 1);
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
            return (T*) ((intptr_t)ptr & (~0x3));
        }
        bool isOwner() const
        {
            return ((intptr_t)ptr & 0x1) == 0;
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
        bool operator==(nullptr_t n) const
        {
            return isNull();
        }
        bool operator!=(nullptr_t n) const
        {
            return isNull() == false;
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
        Pointer<T>& operator = (nullptr_t n)
        {
            clear();
            return *this;
        }

        operator T*()
        {
            return getObject();
        }

        T* operator->()
        {
            T* p = getObject();
            if (p > (void*)0x100000)
            {
                return p;
            }
            throwNullPointExeption();
            return nullptr;
        }
        // operator bool()
        // {
        //     return isNull() == false;
        // }

        // T& operator*()
        // {
        //     return *getObject();
        // }

        void acquire();
        void release();

    public:
        char* ptr = nullptr;
    };
}