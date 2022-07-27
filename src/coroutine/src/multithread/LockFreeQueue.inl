#include <atomic>

namespace OneCoroutine
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class TYPE>
	LockQueue<TYPE>::LockQueue()
	{
        
	}
	template <class TYPE>
	LockQueue<TYPE>::~LockQueue()
	{

	}

	template <class TYPE>
	void LockQueue<TYPE>::push(TYPE const& data)
	{
		_mtx.lock();
		_data.push_back(data);
		_mtx.unlock();
	}

	template <class TYPE>
	bool LockQueue<TYPE>::pop(TYPE& data)
	{
		if (_data.empty())
		{
            return false;
        }

		_mtx.lock();
		if (_data.empty())
		{
			_mtx.unlock();
			return false;
		}
		data = _data.front();
		_data.pop_front();
		_mtx.unlock();
		return true;
	}

	//template <class TYPE>
	//void LockQueue<TYPE>::clear()
	//{
	//	auto_lock<LOCK_TYPE> l(_mtx);
	//	_data.clear();
	//}

	template <class TYPE>
	inline unsigned int LockQueue<TYPE>::size() const
	{
		return (unsigned int)_data.size();
	}

	template <class TYPE>
	inline bool LockQueue<TYPE>::empty() const
	{
		return _data.size() == 0;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <class PUSH_LOCK_TYPE, class POP_LOCK_TYPE, class TYPE>
	LockFreeQueue<PUSH_LOCK_TYPE, POP_LOCK_TYPE, TYPE>::LockFreeQueue(unsigned int maxSize)
	{
		//必须是2的N次方
		_read.set(UINT_MAX - 100);
		_write.set(UINT_MAX - 100);

		_max_size = 0;
		_data = NULL;

		set_max_size(maxSize);
	}

	template <class PUSH_LOCK_TYPE, class POP_LOCK_TYPE, class TYPE>
	LockFreeQueue<PUSH_LOCK_TYPE, POP_LOCK_TYPE, TYPE>::~LockFreeQueue()
	{
		if (_data)
		{
			delete[] _data;
		}
	}

	template <class PUSH_LOCK_TYPE, class POP_LOCK_TYPE, class TYPE>
	void LockFreeQueue<PUSH_LOCK_TYPE, POP_LOCK_TYPE, TYPE>::push(TYPE const& data)
	{
		_write_mtx.lock();

		unsigned int write = (unsigned int)_write.load();
		unsigned int left = write - _read.load();

		//保证都可以放入队列，所以满了就堵塞。
		if ((_reserve.empty() && ((left + 101) < _max_size)))
		{
			_data[write & (_max_size - 1)] = data;
			_write.exchange_add(1);

			_write_mtx.unlock();
		}
		else
		{
			_write_mtx.unlock();

			_reserve.push(data);
		}
	}

	template <class PUSH_LOCK_TYPE, class POP_LOCK_TYPE, class TYPE>
	bool LockFreeQueue<PUSH_LOCK_TYPE, POP_LOCK_TYPE, TYPE>::pop(TYPE& data)
	{
		_read_mtx.lock();

		unsigned int read = _read.load();
		unsigned int left = _write.load() - read;	//必须先计算下
		if (left > 0)
		{
			data = _data[read & (_max_size - 1)];
			_read.exchange_add(1);

			_read_mtx.unlock();

			return true;
		}
		else
		{
			_read_mtx.unlock();

			return _reserve.pop(data);
		}
	}

	template <class PUSH_LOCK_TYPE, class POP_LOCK_TYPE, class TYPE>
	inline unsigned int LockFreeQueue<PUSH_LOCK_TYPE, POP_LOCK_TYPE, TYPE>::_vec_inside_size() const
	{
		unsigned int read_index = (unsigned int)_read.get();
		unsigned int write_index = (unsigned int)_write.get();
		if (UINT_COMPARE(write_index, read_index) >= 0)
		{
			return write_index - read_index;
		}
		else
		{
			return 1;
		}
	}

	template <class PUSH_LOCK_TYPE, class POP_LOCK_TYPE, class TYPE>
	inline unsigned int LockFreeQueue<PUSH_LOCK_TYPE, POP_LOCK_TYPE, TYPE>::size() const
	{
		return _vec_inside_size() + _reserve.size();
	}

	template <class PUSH_LOCK_TYPE, class POP_LOCK_TYPE, class TYPE>
	inline bool LockFreeQueue<PUSH_LOCK_TYPE, POP_LOCK_TYPE, TYPE>::empty() const
	{
		return size() == 0;
	}

	inline unsigned int log2(unsigned int v)
	{
		unsigned int ret = 0;
		if (v & 0xFFFF0000)
		{
			ret += 16;
			v >>= 16;
		}
		if (v & 0xFF00)
		{
			ret += 8;
			v >>= 8;
		}
		if (v & 0xF0)
		{
			ret += 4;
			v >>= 4;
		}
		if (v & 0x0C)
		{
			ret += 2;
			v >>= 2;
		}
		if (v & 0x02)
		{
			ret += 1;
			//v >>= 1;
		}
		return ret;
	}

	template <class PUSH_LOCK_TYPE, class POP_LOCK_TYPE, class TYPE>
	void LockFreeQueue<PUSH_LOCK_TYPE, POP_LOCK_TYPE, TYPE>::set_max_size(unsigned int size)
	{
		_max_size = 1 << log2(std::max(size, 1024u));
		if (_max_size < size)
		{
			_max_size = _max_size << 1;
		}
		if (_data)
		{
			delete[] _data;
		}
		_data = new(std::nothrow) TYPE[_max_size];
	}
}