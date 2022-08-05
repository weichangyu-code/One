#include "Atomic.h"

namespace OneCoroutine
{
	intptr_t AtomicApi::comp_exchange(volatile intptr_t* value, intptr_t comp, intptr_t exchange)
    {
		return __sync_val_compare_and_swap(value, comp, exchange);
    }
	intptr_t AtomicApi::exchange(volatile intptr_t* value, intptr_t exchange)
    {
		return __sync_lock_test_and_set(value, exchange);
    }
	intptr_t AtomicApi::exchange_add(volatile intptr_t* value, intptr_t add)
    {
		return __sync_fetch_and_add(value, add);
    }
    
    // Atomic::Atomic(intptr_t count)
    // {
    //     this->count = count;
    // }

    //     //非原子操作
    
    // intptr_t Atomic::get()
    // {
    //     return count;
    // }
        
    // void Atomic::set(intptr_t count)
    // {
    //     this->count = count;
    // }

	// intptr_t Atomic::comp_exchange(intptr_t comp, intptr_t exchange)
    // {
	// 	return __sync_val_compare_and_swap(&count, comp, exchange);
    // }
	// intptr_t Atomic::exchange(intptr_t exchange)
    // {
	// 	return __sync_lock_test_and_set(&count, exchange);
    // }
	// intptr_t Atomic::exchange_add(intptr_t add)
    // {
	// 	return __sync_fetch_and_add(&count, add);
    // }
    // intptr_t Atomic::load()
    // {
    //     return __sync_fetch_and_add(&count, 0);
    // }
    // void Atomic::store(intptr_t count)
    // {
    //     __sync_lock_test_and_set(&this->count, count);
    // }

}