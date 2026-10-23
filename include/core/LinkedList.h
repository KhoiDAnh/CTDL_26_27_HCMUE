#pragma once

#include <cstddef>
#include <stdexcept>

// NOTE: temp file to run ChuyenBay.h, subject to CHANGE
// replace with M1's version if it gets updated

// LinkedList: Nodes && Nodes:data+ptr
// LinkedList should be the ONLY one to read the Nodes
template <typename T>
class LinkedList {
private:
    struct Node {
        // Holds all the typename data and ptr
        explicit Node(const T& value)
        : data(value), next(nullptr) {}

        // outside else it becomes throwaway local vars
        T data;
        Node* next;
    };

    Node* nodeAt(std::size_t index) const;
    void clear();

    Node* head_;
    Node* tail_;
    std::size_t size_;
public:
    // copy constructor, copy assignment operator, destructor
    LinkedList();
    LinkedList(const LinkedList& other);
    LinkedList& operator=(const LinkedList& other);
    ~LinkedList();

    void pushBack(const T& data);
    std::size_t size() const;
    bool isEmpty() const;
    T& at(std::size_t index);
    const T& at(std::size_t index) const;
    void removeAt(std::size_t index);
};
// child Node methods in LList
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::nodeAt(std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("index out of range");
    }
    Node* current = head_;
    for (std::size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current;
}

template <typename T>
void LinkedList<T>::clear() {
    Node* current = head_;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head_ = tail_ = nullptr;
    size_ = 0;
}

template <typename T>
LinkedList<T>::LinkedList()
    : head_(nullptr), tail_(nullptr), size_(0) {}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other)
    : head_(nullptr), tail_(nullptr), size_(0) {
    for (Node* current = other.head_; current != nullptr; current = current->next) {
        pushBack(current->data);
    }
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {

    // 1. Self assigment guard
    if (this == &other) {
        return *this;
    }

    // 2. Clear
    this->clear();

    // 3. copy -> loop -> pushBack
    Node* current = other.head_;
    while (current != nullptr) {
        this->pushBack(current->data);
        current = current->next;
    }

    // 4. end by returning *this
    return *this;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

template <typename T>
void LinkedList<T>::pushBack(const T& data) {
    Node* newNode = new Node(data);
    if (isEmpty()) {
        head_ = newNode;
    }
    else {
        tail_->next = newNode;
    }
    tail_ = newNode;
    ++size_;
}

template <typename T>
std::size_t LinkedList<T>::size() const {
    return size_;
}

template <typename T>
bool LinkedList<T>::isEmpty() const {
    return size_ == 0;
}

template <typename T>
T& LinkedList<T>::at(std::size_t index) {
    return nodeAt(index)->data;
}

template <typename T>
const T& LinkedList<T>::at(std::size_t index) const {
    return nodeAt(index)->data;
}

template <typename T>
void LinkedList<T>::removeAt(std::size_t index) {
    // throws std::out_of_range if index is invalid
    nodeAt(index);

    if (index == 0) {
        Node* oldHead = head_;
        head_ = head_->next;
        delete oldHead;
        if (head_ == nullptr) {
            tail_ = nullptr;
        }
    }
    else {
        Node* prev = nodeAt(index - 1);
        Node* target = prev->next;

        prev->next = target->next;

        if (target == tail_) {
            tail_ = prev;
        }
        delete target;
    }
    --size_;
}