#pragma once
#include <stdint.h>
#include <bits/move.h>
#include <cstddef>

namespace One
{
    //T可以是Object或者Interface，或者他们的子类
    template<typename T>
    class Pointer
    {
    public:
        Pointer()
        {}
        Pointer(nullptr_t n)
        {}
        Pointer(T* ptr, bool inner)
        {
            if (ptr)
            {
                this->ptr = (char*)ptr + (inner ? 1 : 0);
            }
        }
        Pointer(const Pointer<T>& r)
        {
            this->ptr = r.ptr;
        }

        // template<typename TT>
        // Pointer(const Pointer<TT>& r)
        // {
        //     set(r.getObject(), r.isInner());
        // }

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
            return (T*) ((intptr_t)ptr & (~0x3));
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

        void acquire();
        void release();
        
        Pointer<T> toOwnerPointer()
        {
            return Pointer<T>(getObject(), false);
        }
        Pointer<T> toInnerPointer()
        {
            return Pointer<T>(getObject(), true);
        }

    public:
        char* ptr = nullptr;
    };
}