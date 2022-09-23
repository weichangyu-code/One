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
        Reference(T* obj)
        {
            pointer.set(obj, true);
            if (obj)
            {
                pointer.acquire();
            }
        }
        Reference(T* obj, bool owner, bool acquire)
        {
            pointer.set(obj, owner);
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

        void set(T* obj, bool owner, bool acquire)
        {
            //要先加索引再减，避免自己赋值给自己，把自己销毁了
            Pointer<T> pointer(obj, owner);
            if (obj && acquire)
            {
                pointer.acquire();
            }
            clear();
            this->pointer = pointer;
        }
        void set(const Pointer<T>& r, bool acquire)
        {
            //先拷贝出来，避免被clear
            Pointer<T> pointer = r;
            if (pointer && acquire)
            {
                pointer.acquire();
            }
            clear();
            this->pointer = pointer;
        }
        void set(const Reference<T>& r)
        {
            Pointer<T> pointer = r.pointer;
            if (pointer)
            {
                pointer.acquire();
            }
            clear();
            this->pointer = pointer;
        }
        void set(Reference<T>&& r)
        {
            Pointer<T> pointer = r.pointer;
            r.pointer.clear();
            clear();
            this->pointer = pointer;
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
        bool isOwner() const
        {
            return pointer.isOwner();
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

    public:
        Pointer<T> pointer;
    };
}
