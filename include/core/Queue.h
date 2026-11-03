#ifndef CTDL_26_27_HCMUE_QUEUE_H
#define CTDL_26_27_HCMUE_QUEUE_H

#include "core/LinkedList.h"

template <typename T>
class Queue {
private:
    LinkedList<T> data_;

public:
    void enqueue(const T& value);
    void dequeue();

    T& front();
    const T& front() const;

    bool isEmpty() const;
    std::size_t size() const;
};

template <typename T>
void Queue<T>::enqueue(const T& value) {
    data_.pushBack(value);
}

template <typename T>
void Queue<T>::dequeue() {
    data_.popFront();
}

template <typename T>
T& Queue<T>::front() {
    return data_.front();
}

template <typename T>
const T& Queue<T>::front() const {
    return data_.front();
}

template <typename T>
bool Queue<T>::isEmpty() const {
    return data_.isEmpty();
}

template <typename T>
std::size_t Queue<T>::size() const {
    return data_.size();
}

#endif
