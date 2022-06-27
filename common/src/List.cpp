#include "List.h"

namespace OneCommon
{
	ListNode::ListNode()
	{
        _head = nullptr;
		_next = _prev = nullptr;
	}

	bool ListNode::inList() const
	{
		return _head != nullptr;
	}
		
    bool ListNode::inList(ListHead* head) const
    {
        return _head == head;
    }
        
    void ListNode::pop()
    {
        if (_head)
        {
            _head->_pop(this);
        }
    }

	const ListNode* ListNode::next() const
	{
		return (_next == this) ? nullptr : _next;
	}

	const ListNode* ListNode::prev() const
	{
		return (_prev == this) ? nullptr : _prev;
	}

	ListNode* ListNode::next()
	{
		return (_next == this) ? nullptr : _next;
	}

	ListNode* ListNode::prev()
	{
		return (_prev == this) ? nullptr : _prev;
	}
		
	const ListHead* ListNode::head() const
	{
		return _head;
	}
		
	ListHead* ListNode::head()
	{
		return _head;
	}

	const ListNode* ListHead::head() const
	{
		return _head;
	}

	const ListNode* ListHead::tail() const
	{
		return _tail;
	}

	ListNode* ListHead::head()
	{
		return _head;
	}

	ListNode* ListHead::tail()
	{
		return _tail;
	}

	bool ListHead::empty() const
	{
		return _num == 0;
	}

	unsigned int ListHead::size() const
	{
		return _num;
	}


	ListHead::ListHead()
	{
		_head = _tail = nullptr;
		_num = 0;
	}

	void ListHead::pushTail(ListNode* node)
	{
        if (node->inList() == false)
        {
			node->_head = this;
			_num++;
			if (_head)
			{
				//加入到列表尾部
				_tail->_next = node;
				node->_prev = _tail;
				node->_next = node;
				_tail = node;
			}
			else
			{
				_head = _tail = node;
				node->_next = node->_prev = node;
			}
        }
	}
	
	void ListHead::pushHead(ListNode* node)
	{
        if (node->inList() == false)
        {
			node->_head = this;
			_num++;
			if (_head)
			{
				//加入到列表头部
				_head->_prev = node;
				node->_next = _head;
				node->_prev = node;
				_head = node;
			}
			else
			{
				_head = _tail = node;
				node->_next = node->_prev = node;
			}
        }
	}

	void ListHead::pop(ListNode* node)
	{
		if (node->inList(this))
		{
			_pop(node);
		}
	}
		
	void ListHead::_pop(ListNode* node)
	{
		if ((_head == node) && (_tail == node))
		{
			_head = _tail = nullptr;
		}
		else if (_head == node)
		{
			_head = node->_next;
			_head->_prev = _head;
		}
		else if (_tail == node)
		{
			_tail = node->_prev;
			_tail->_next = _tail;
		}
		else
		{
			node->_next->_prev = node->_prev;
			node->_prev->_next = node->_next;
		}
		_num--;

		node->_next = node->_prev = nullptr;
        node->_head = nullptr;
	}
	
	void ListHead::insertBefore(ListNode* pos, ListNode* node)
	{
        if (node->inList() || pos->inList(this) == false)
        {
            return;
        }
        node->_head = this;

		if (pos == _head)
		{
			pushHead(node);
			return;
		}
		if (pos == nullptr)
		{
			pushTail(node);
			return;
		}
		ListNode* prev_pos = pos->_prev;
		prev_pos->_next = node;
		node->_next = pos;
		pos->_prev = node;
		node->_prev = prev_pos;
		_num++;
	}

	void ListHead::insertAfter(ListNode* pos, ListNode* node)
	{
        if (node->inList() || pos->inList(this) == false)
        {
            return;
        }
        node->_head = this;

		if (pos == _tail)
		{
			pushTail(node);
			return;
		}
		if (pos == nullptr)
		{
			pushHead(node);
			return;
		}
		ListNode* next_pos = pos->_next;
		pos->_next = node;
		node->_next = next_pos;
		next_pos->_prev = node;
		node->_prev = pos;
		_num++;
	}

	// void ListHead::append(ListHead* list)
	// {
	// 	if (list->_head == nullptr)
	// 	{
	// 		return;
	// 	}

	// 	if (_head == nullptr)
	// 	{
	// 		_head = list->_head;
	// 		_tail = list->_tail;
	// 	}
	// 	else
	// 	{
	// 		_tail->_next = list->_head;
	// 		list->_head->_prev = _tail;
	// 		_tail = list->_tail;
	// 	}
	// 	_num += list->_num;

	// 	list->_head = list->_tail = nullptr;
	// 	list->_num = 0;
	// }
	
	// void ListHead::swap(ListHead* list)
	// {
	// 	std::swap(_head, list->_head);
	// 	std::swap(_tail, list->_tail);
	// 	std::swap(_num, list->_num);
	// }

	void ListHead::clear()
	{
		ListNode* node = _head;
		while (node)
		{
			ListNode* next = node->next();

            node->_head = nullptr;
			node->_next = node->_prev = nullptr;

			node = next;
		}
		_head = _tail = nullptr;
		_num = 0;
	}

} // namespace One
