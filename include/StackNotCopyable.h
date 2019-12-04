#pragma once

#include "StackAbstract.h"


template<class T>
class StackNotCopyable: public StackAbstract<T>
{
public:
    template<typename ... Args>
    void push_emplace(Args &&... value)
    {
        auto *prevNode = StackAbstract<T>::headNode;

        StackAbstract<T>::headNode = new StackAbstractNode<T>{
            {std::forward<Args>(value)...},
            prevNode,
        };
    }

    T pop()
    {
        if (!StackAbstract<T>::headNode) {
            throw StackEmptyError("Stack is empty");
        }

        auto *prevNode = StackAbstract<T>::headNode;
        StackAbstract<T>::headNode = StackAbstract<T>::headNode->prev;

        T value = std::move(prevNode->value);
        delete prevNode;
        return value;
    }
};