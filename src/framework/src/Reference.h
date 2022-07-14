#pragma once
#include "Pointer.h"

namespace One
{
    template<typename T>
    class Reference
    {
    public:
        Reference()
        {
        }
        Reference(nullptr_t n)
        {
        }
        ~Reference()
        {
            if (pointer)
            {
                pointer.release();
            }
        }
        Reference(T* obj, bool inner, bool acquire)
        {
            pointer.set(obj, inner);
            if (obj && acquire)
            {
                pointer.acquire();
            }
        }
        Reference(const Pointer<T>& r, bool acquire)
        {
            pointer = r;
            if (pointer && acquire)
            {
                pointer.acquire();
            }
        }
        Reference(const Pointer<T>& r)
        {
            pointer = r;
            if (pointer)
            {
                pointer.acquire();
            }
        }
        Reference(const Reference<T>& r)
        {
            pointer = r.pointer;
            if (pointer)
            {
                pointer.acquire();
            }
        }
        Reference(Reference<T>&& r)
        {
            pointer = r.pointer;
            r.pointer.clear();
        }

        //支持不同类型的转换，需要手动转换
        // template<typename TT>
        // Reference(const Reference<TT>& r)
        // {
        //     pointer.set(r.pointer.getObject(), r.pointer.isInner());
        //     if (pointer)
        //     {
        //         pointer.acquire();
        //     }
        // }
        // template<typename TT>
        // Reference(Reference<TT>&& r)
        // {
        //     pointer.set(r.pointer.getObject(), r.pointer.isInner());
        //     r.pointer.clear();
        // }

        void set(T* obj, bool inner, bool acquire)
        {
            clear();
            pointer.set(obj, inner);
            if (obj && acquire)
            {
                pointer.acquire();
            }
        }
        void set(const Pointer<T>& r, bool acquire)
        {
            clear();
            pointer = r;
            if (pointer && acquire)
            {
                pointer.acquire();
            }
        }
        void set(const Reference<T>& r)
        {
            clear();
            pointer = r.pointer;
            if (pointer)
            {
                pointer.acquire();
            }
        }
        void set(Reference<T>&& r)
        {
            clear();
            pointer = r.pointer;
            r.pointer.clear();
        }
        void clear()
        {
            if (pointer)
            {
                pointer.release();
                pointer.clear();
            }
        }
        Pointer<T> detach()
        {
            Pointer<T> pt = this->pointer;
            this->pointer.clear();
            return pt;
        }
        
        T* getObject() const
        {
            return pointer.getObject();
        }
        Pointer<T>& getPointer() const
        {
            return pointer;
        }
        bool isInner() const
        {
            return pointer.isInner();
        }
        bool isNull() const
        {
            return pointer.isNull();
        }

        Reference<T>& operator = (const Reference<T>& r)
        {
            set(r);
            return *this;
        }
        Reference<T>& operator = (const Pointer<T>& r)
        {
            set(r, true);
            return *this;
        }
        Reference<T>& operator = (T* r)
        {
            set(r, false, true);
            return *this;
        }
        Reference<T>& operator = (Reference<T>&& r)
        {
            set(std::move(r));
            return *this;
        }
        Reference<T>& operator = (nullptr_t n)
        {
            clear();
            return *this;
        }
        bool operator==(const Reference<T>& r) const
        {
            return getObject() == r.getObject();
        }
        bool operator!=(const Reference<T>& r) const
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
        bool operator>(const Reference<T>& r) const
        {
            return getObject() > r.getObject();
        }
        bool operator<(const Reference<T>& r) const
        {
            return getObject() < r.getObject();
        }

        operator T*()
        {
            return getObject();
        }
        operator Pointer<T>()
        {
            return pointer;
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

        Reference<T> toOwnerReference()
        {
            return Reference<T>(pointer->toOwnerPointer(), true);
        }
        Reference<T> toInnerReference()
        {
            return Reference<T>(pointer->toInnerPointer(), true);
        }

    public:
        Pointer<T> pointer;
    };

    template<typename T1, typename T2>
    Reference<T2>& convertObjectReference(const Reference<T1>& r)
    {
        return (Reference<T2>&)r;
    }

    template<typename T1, typename T2>
    Reference<T2> convertInterfaceReference(const Reference<T1>& r)
    {
        return Reference<T2>(r.getObject(), r.isInner(), true);
    }
}
