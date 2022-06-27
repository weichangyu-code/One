#pragma once

namespace OneCoroutine
{
    //智能对象，线程内使用
    class AutoObject
    {
    public:
        void acquire()
        {
            this->count++;
        }
        void release()
        {
            if (--count == 0)
            {
                onDestruct();
            }
        }
        void resetRefCount()
        {
            this->count = 1;
        }

    protected:
        virtual void onDestruct()
        {
            delete this;
        }

    protected:
        int count = 1;
    };

    template<typename T>
    class AutoPtr
    {
    public:
        AutoPtr()
        {
        }
        ~AutoPtr()
        {
            clear();
        }
        AutoPtr(T* obj, bool acquire = true)
        {
            this->p = obj;
            if (this->p && acquire)
            {
                this->p->acquire();
            }
        }
        AutoPtr(const AutoPtr<T>& r)
        {
            this->p = r.p;
            if (this->p)
            {
                this->p->acquire();
            }
        }
        AutoPtr(AutoPtr<T>&& r)
        {
            this->p = r.p;
            r.p = nullptr;
        }

        void set(T* obj, bool acquire)
        {
            clear();
            this->p = obj;
            if (this->p && acquire)
            {
                this->p->acquire();
            }
        }
        void set(const AutoPtr<T>& r)
        {
            clear();
            this->p = r.p;
            if (this->p)
            {
                this->p->acquire();
            }
        }
        void set(AutoPtr<T>&& r)
        {
            clear();
            this->p = r.p;
            r.p = nullptr;
        }
        void clear()
        {
            if (this->p)
            {
                this->p->release();
                this->p = nullptr;
            }
        }
        T* detach()
        {
            T* p = this->p;
            this->p = nullptr;
            return this->p;
        }
        
        T* getObject() const
        {
            return this->p;
        }
        bool isNull() const
        {
            return this->p == nullptr;
        }

        AutoPtr<T>& operator = (const AutoPtr<T>& r)
        {
            set(r);
            return *this;
        }
        AutoPtr<T>& operator = (const T* r)
        {
            set(r, true);
            return *this;
        }
        AutoPtr<T>& operator = (AutoPtr<T>&& r)
        {
            set(std::move(r));
            return *this;
        }
        bool operator==(const AutoPtr<T>& r) const
        {
            return getObject() == r.getObject();
        }
        bool operator!=(const AutoPtr<T>& r) const
        {
            return getObject() != r.getObject();
        }
        bool operator>(const AutoPtr<T>& r) const
        {
            return getObject() > r.getObject();
        }
        bool operator<(const AutoPtr<T>& r) const
        {
            return getObject() < r.getObject();
        }

        // 不能支持转，避免直接用指针赋值，导致野指针
        // operator T*()
        // {
        //     return getObject();
        // }
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
        T* p = nullptr;
    };

} // namespace One

