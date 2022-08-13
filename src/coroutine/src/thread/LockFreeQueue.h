#pragma once
#include <deque>
#include <mutex>
#include "Atomic.h"
using namespace std;

namespace OneCoroutine
{
    //假锁
    class false_mutex
    {
    public:
        void lock()
        {
        }
        void unlock()
        {
        }
    };

	/*
	stl队列+锁
	*/
	template <class TYPE>
	class LockQueue
	{
	public:
		LockQueue();
		~LockQueue();

	public:
		void push(TYPE const& data);
		bool pop(TYPE& data);

		inline unsigned int size() const;
		inline bool empty() const;

	private:
		mutex _mtx;
		deque<TYPE> _data;
	};

	template <class PUSH_LOCK_TYPE, class POP_LOCK_TYPE, class TYPE>
	class LockFreeQueue
	{
	public:
		LockFreeQueue(unsigned int maxSize = 4096);
		~LockFreeQueue();

	public:
		void push(TYPE const& data);
		bool pop(TYPE& data);

		inline unsigned int size() const;
		inline bool empty() const;

		//
	protected:
		void set_max_size(unsigned int size);
		inline unsigned int _vec_inside_size() const;

	private:
		unsigned int _max_size;
		TYPE* _data;

		POP_LOCK_TYPE _read_mtx;
		AtomicInt _read;
		PUSH_LOCK_TYPE _write_mtx;
		AtomicInt _write;

		//超出固定大小的时候，防止死循环。
		LockQueue<TYPE> _reserve;
	};

}

#include "LockFreeQueue.inl"