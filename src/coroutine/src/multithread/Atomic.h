#pragma once
#include "Common.h"

namespace OneCoroutine
{
    class AtomicApi
    {
    public:
		static intptr_t comp_exchange(volatile intptr_t* value, intptr_t comp, intptr_t exchange);		//返回变化前的值
		static intptr_t exchange(volatile intptr_t* value, intptr_t exchange);						    //同上
		static intptr_t exchange_add(volatile intptr_t* value, intptr_t add);						    //同上
    };

    template<typename T>
    class AtomicPtr
    {
    public:
        T* get() const
        {
            return (T*)_p;
        }

        void set(T* p)
        {
            _p = p;
        }

		T* comp_exchange(T* comp, T* exchange)
        {
            return (T*)AtomicApi::comp_exchange((volatile intptr_t*)&_p, (intptr_t)comp, (intptr_t)exchange);
        }
		T* exchange(T* exchange)
        {
            return (T*)AtomicApi::exchange((volatile intptr_t*)&_p, (intptr_t)exchange);
        }
        T* load()
        {
            return (T*)AtomicApi::exchange_add((volatile intptr_t*)&_p, 0);
        }
        void store(T* v)
        {
            AtomicApi::exchange((volatile intptr_t*)&_p, (intptr_t)v);
        }

    protected:
        volatile T* _p = nullptr;
    };

    class AtomicInt
    {
    public:
        intptr_t get() const
        {
            return _d;
        }

        void set(intptr_t d)
        {
            _d = d;
        }

		intptr_t comp_exchange(intptr_t comp, intptr_t exchange)
        {
            return AtomicApi::comp_exchange((volatile intptr_t*)&_d, comp, exchange);
        }
		intptr_t exchange(intptr_t exchange)
        {
            return AtomicApi::exchange((volatile intptr_t*)&_d, exchange);
        }
		intptr_t exchange_add(intptr_t v)
        {
            return AtomicApi::exchange_add((volatile intptr_t*)&_d, v);
        }
        intptr_t load()
        {
            return AtomicApi::exchange_add((volatile intptr_t*)&_d, 0);
        }
        void store(intptr_t v)
        {
            AtomicApi::exchange((volatile intptr_t*)&_d, v);
        }

    protected:
        volatile intptr_t _d = 0;
    };

    // class Atomic
    // {
    // public:
    //     Atomic(intptr_t count);

    //     //非原子操作
    //     intptr_t get();
    //     void set(intptr_t count);

	// 	intptr_t comp_exchange(intptr_t comp, intptr_t exchange);		//返回变化前的值
	// 	intptr_t exchange(intptr_t exchange);						//同上
	// 	intptr_t exchange_add(intptr_t add);						//同上
    //     intptr_t load();
    //     void store(intptr_t count);

    // protected:
    //     volatile intptr_t count;
    // };
}
