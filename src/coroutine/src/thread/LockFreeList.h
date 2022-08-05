#pragma once
#include "Atomic.h"

/**
 * List型无锁队列的特征要求：
 * 1. Node节点在没有被局部变量指向，没有被NEXT指向的时候，才能删除。这个JAVA可以做到，C++要用智能指针，或者对象永远不删除
 * 2. Node节点不要重复使用，不然push又pop进来，状态更复杂
 * 3. 如果NODE节点放入内存池，那内存池也不能加锁。不然就失去无锁队列的意义了
 * 4. 可以CAS有限自旋，不能因为另一个线程执行切换，而无限自旋。不能类似自旋锁，不然就是锁了
 */

namespace OneCoroutine
{
    class LockFreeNode
    {
    public: 
        LockFreeNode* next()
        {
            return _next.get();
        }

    public:
        AtomicPtr<LockFreeNode> _next;
    };

    class LockFreeStackHead
    {
    public:
        LockFreeNode* pop();
        void push(LockFreeNode* node);

        //线程不安全
        LockFreeNode* popUnsafe();
        void pushUnsafe(LockFreeNode* node);

        LockFreeNode* head()
        {
            return _head.get();
        }

    protected:
        AtomicPtr<LockFreeNode> _head;
    };

    // 没成功
    // class LockFreeListHead
    // {
    // public:
    //     LockFreeNode* pop();
    //     void push(LockFreeNode* node);

    // protected:
    //     AtomicPtr<LockFreeNode> _head;
    //     AtomicPtr<LockFreeNode> _tail;
    // };

}