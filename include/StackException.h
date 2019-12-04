#pragma once

#include <stdexcept>

class StackException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class StackEmptyError : public StackException {
public:
    using StackException::StackException;
};