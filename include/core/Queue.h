#pragma once

#include <cstddef>

#include "core/LinkedList.h"

// First-in, first-out line built on LinkedList. Holds the waiting customers.
template <typename T>
class Queue {
private:
    LinkedList<T> data_;
public:
    void enqueue(const T& value);
    void dequeue();
    T& front();
    const T& front() const;
    const T& at(std::size_t index) const;
    bool isEmpty() const;
    std::size_t size() const;
};

template <typename T>
void Queue<T>::enqueue(const T& value) {
    data_.pushBack(value);
}

template <typename T>
void Queue<T>::dequeue() {
    data_.removeAt(0);  // throws std::out_of_range when empty
}

template <typename T>
T& Queue<T>::front() {
    return data_.at(0);
}

template <typename T>
const T& Queue<T>::front() const {
    return data_.at(0);
}

template <typename T>
const T& Queue<T>::at(std::size_t index) const {
    return data_.at(index);
}

template <typename T>
bool Queue<T>::isEmpty() const {
    return data_.isEmpty();
}

template <typename T>
std::size_t Queue<T>::size() const {
    return data_.size();
}