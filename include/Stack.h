#pragma once

#include "StackAbstract.h"


template<class T>
class Stack: public StackAbstract<T>
{
public:
    void push(const T &value)
    {
        auto *prevNode = StackAbstract<T>::headNode;

        StackAbstract<T>::headNode = new StackAbstractNode<T>{
            value,
            prevNode,
        };
    }

    void pop()
    {
        if (!StackAbstract<T>::headNode) {
            throw StackEmptyError("Stack is empty");
        }

        auto *prevNode = StackAbstract<T>::headNode;
        StackAbstract<T>::headNode = StackAbstract<T>::headNode->prev;

        delete prevNode;
    }
};