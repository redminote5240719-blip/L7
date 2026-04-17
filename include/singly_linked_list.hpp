#pragma once

#include <cstddef>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <utility>

#include "node.hpp"

template <typename T>
class SinglyLinkedList {
private:
    using NodeT = Node<T>;

    std::shared_ptr<NodeT> head_{};
    std::shared_ptr<NodeT> tail_{};
    std::size_t size_ = 0;

    std::shared_ptr<NodeT> node_at(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("SinglyLinkedList::at: index out of range");
        }
        auto cur = head_;
        for (std::size_t i = 0; i < index; ++i) {
            cur = cur->next;
        }
        return cur;
    }

public:
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);

    SinglyLinkedList() = default;

    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
    [[nodiscard]] std::size_t size() const noexcept { return size_; }

    T& at(std::size_t index) { return node_at(index)->value; }
    const T& at(std::size_t index) const { return node_at(index)->value; }

    T& operator[](std::size_t index) { return at(index); }
    const T& operator[](std::size_t index) const { return at(index); }

    void insert(std::size_t index, T value) {
        if (index > size_) {
            throw std::out_of_range("SinglyLinkedList::insert: index out of range");
        }
        if (index == 0) {
            push_front(std::move(value));
            return;
        }
        if (index == size_) {
            push_back(std::move(value));
            return;
        }

        auto prev = node_at(index - 1);
        auto node = std::make_shared<NodeT>(std::move(value));
        node->next = prev->next;
        prev->next = std::move(node);
        ++size_;
    }

    void erase(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("SinglyLinkedList::erase: index out of range");
        }
        if (index == 0) {
            pop_front();
            return;
        }

        auto prev = node_at(index - 1);
        auto target = prev->next;
        prev->next = target ? target->next : nullptr;
        if (target == tail_) {
            tail_ = prev;
        }
        --size_;
    }

    [[nodiscard]] std::size_t index_of(const T& value) const {
        std::size_t idx = 0;
        auto cur = head_;
        while (cur) {
            if (cur->value == value) {
                return idx;
            }
            cur = cur->next;
            ++idx;
        }
        return npos;
    }

    void push_front(T value) {
        auto node = std::make_shared<NodeT>(std::move(value));
        node->next = head_;
        head_ = std::move(node);
        if (!tail_) {
            tail_ = head_;
        }
        ++size_;
    }

    void push_back(T value) {
        auto node = std::make_shared<NodeT>(std::move(value));
        if (!tail_) {
            head_ = node;
            tail_ = std::move(node);
            size_ = 1;
            return;
        }
        tail_->next = node;
        tail_ = std::move(node);
        ++size_;
    }

    void pop_front() {
        if (empty()) {
            throw std::out_of_range("SinglyLinkedList::pop_front: list is empty");
        }
        head_ = head_->next;
        --size_;
        if (size_ == 0) {
            tail_.reset();
        }
    }

    void pop_back() {
        if (empty()) {
            throw std::out_of_range("SinglyLinkedList::pop_back: list is empty");
        }
        if (head_ == tail_) {
            head_.reset();
            tail_.reset();
            size_ = 0;
            return;
        }

        auto prev = head_;
        while (prev->next != tail_) {
            prev = prev->next;
        }
        prev->next.reset();
        tail_ = std::move(prev);
        --size_;
    }

    void clear() noexcept {
        head_.reset();
        tail_.reset();
        size_ = 0;
    } 
    

    friend std::ostream& operator<<(std::ostream& os, const SinglyLinkedList& list) {
        os << "[";
        auto cur = list.head_;
        while (cur) {
            os << cur->value;
            cur = cur->next;
            if (cur) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }
};