#include "LockFreeList.h"

namespace OneCoroutine
{

    LockFreeNode* LockFreeStackHead::pop()
    {
        do
        {
            //其实这一步可以不用原子操作，可惜ATOMIC类不提供接口
            LockFreeNode* head = _head.load();
            if (head == nullptr)
            {
                return nullptr;
            }
            //直接访问node->next有一定的风险，所以要求node不能销毁
            if (_head.comp_exchange(head, head->_next.get()) == head)
            {
                return head;
            }
        } while (1);
        return nullptr;
    }
        
    void LockFreeStackHead::push(LockFreeNode* node)
    {
        do
        {
            LockFreeNode* head = _head.get();
            node->_next.set(head);
            //直接访问node->next有一定的风险，所以要求node不能销毁
            if (_head.comp_exchange(head, node))
            {
                return;
            }
        } while (1);
    }
        
    LockFreeNode* LockFreeStackHead::popUnsafe()
    {
        LockFreeNode* head = _head.get();
        if (head == nullptr)
        {
            return nullptr;
        }
        _head.set(head->_next.get());
        return head;
    }
        
    void LockFreeStackHead::pushUnsafe(LockFreeNode* node)
    {
        node->_next.set(_head.get());
        _head.set(node);
    }

        
    // LockFreeNode* LockFreeListHead::pop()
    // {
    //     do
    //     {
    //         LockFreeNode* head = _head.load();
    //         if (head == nullptr)
    //         {
    //             return nullptr;
    //         }
    //         if (_head.comp_exchange(head, head->_next.get()) == head)
    //         {
    //             _tail.comp_exchange(head, nullptr);
    //             return head;
    //         }
    //     } while (1);
    // }
        
    // void LockFreeListHead::push(LockFreeNode* node)
    // {
    //     node->_next.set(nullptr);
    //     do
    //     {
    //         LockFreeNode* tail = _tail.get();
    //         if (tail == nullptr)
    //         {
    //             if (_tail.comp_exchange(tail, node) == tail)
    //             {
    //                 _head.store(node);
    //                 return;
    //             }
    //         }
    //         else
    //         {
    //             if (_tail.comp_exchange(tail, node) == tail)
    //             {
    //                 tail->_next.store(node);
    //                 _head.comp_exchange(nullptr, node);
    //                 return;
    //             }
    //         }
    //     } while (1);
    // }
} // namespace OneCommon
