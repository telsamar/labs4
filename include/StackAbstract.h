#pragma once

#include <type_traits>
#include <utility>
#include "StackException.h"

template<class T>
struct StackAbstractNode
{
    T value;
    StackAbstractNode *prev = nullptr;
};

template<class T>
class StackAbstract
{
public:
    StackAbstract() = default;
    StackAbstract(const StackAbstract &stack) = delete;
    StackAbstract(StackAbstract &&stack) noexcept = default;

    StackAbstract &operator=(const StackAbstract &stack) = delete;
    StackAbstract &operator=(StackAbstract &&stack) noexcept = default;

    void push(T &&value)
    {
        auto *prevNode = headNode;

        headNode = new StackAbstractNode<T>{
            std::forward<T>(value),
            prevNode,
        };
    }

    const T &head() const
    {
        if (!headNode) {
            throw StackEmptyError("Stack is empty");
        }

        return headNode->value;
    }

    ~StackAbstract()
    {
        while (headNode) {
            auto *currentNode = headNode;
            headNode = currentNode->prev;

            delete currentNode;
        }
    }

protected:
    StackAbstractNode<T> *headNode = nullptr;
};