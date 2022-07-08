#pragma once

namespace OneCommon
{

#define GET_OFFSET(TYPE, mem)				((unsigned int)(intptr_t)&((TYPE*)0)->mem)
//#define GET_OFFSET(TYPE, mem)				(offsetof(TYPE, mem))
#define GET_ENTRY(TYPE, mem, p)				(TYPE*)((char*)p - GET_OFFSET(TYPE, mem))
#define GET_CONST_ENTRY(TYPE, mem, p)		(const TYPE*)((char*)p - GET_OFFSET(TYPE, mem))

    class ListHead;
	class ListNode
	{
		friend class ListHead;
	public:
		ListNode();

	public:
		bool inList() const;
		bool inList(ListHead* head) const;
        void pop();

		const ListNode* next() const;
		const ListNode* prev() const;
		ListNode* next();
		ListNode* prev();

		const ListHead* head() const;
		ListHead* head();

	private:
        ListHead* _head;
		ListNode* _next;
		ListNode* _prev;
	};

	class ListHead
	{
		friend class ListNode;
	public:
		ListHead();

	public:
		void pushTail(ListNode* node);
		void pushHead(ListNode* node);
		void pop(ListNode* node);
		void insertBefore(ListNode* pos, ListNode* node);
		void insertAfter(ListNode* pos, ListNode* node);

		const ListNode* head() const;
		const ListNode* tail() const;
		ListNode* head();
		ListNode* tail();

		//void append(ListHead* list);
		void clear();
		//void swap(ListHead* list);

		bool empty() const;
		unsigned int size() const;

	protected:
		void _pop(ListNode* node);

	private:
		ListNode* _head;
		ListNode* _tail;
		unsigned int _num;
	};
}
