#pragma once

#include <cstddef>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <utility>

#include "node.hpp"

template <typename T>
class DoublyLinkedList {
private:
    using NodeT = Node<T>;

    std::shared_ptr<NodeT> head_{};
    std::shared_ptr<NodeT> tail_{};
    std::size_t size_ = 0;

    std::shared_ptr<NodeT> node_at(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("DoublyLinkedList::at: index out of range");
        }
        auto cur = head_;
        for (std::size_t i = 0; i < index; ++i) {
            cur = cur->next;
        }
        return cur;
    }

public:
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);

    DoublyLinkedList() = default;

    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
    [[nodiscard]] std::size_t size() const noexcept { return size_; }

    T& at(std::size_t index) { return node_at(index)->value; }
    const T& at(std::size_t index) const { return node_at(index)->value; }

    T& operator[](std::size_t index) { return at(index); }
    const T& operator[](std::size_t index) const { return at(index); }

    void insert(std::size_t index, T value) {
        if (index > size_) {
            throw std::out_of_range("DoublyLinkedList::insert: index out of range");
        }
        if (index == 0) {
            push_front(std::move(value));
            return;
        }
        if (index == size_) {
            push_back(std::move(value));
            return;
        }

        auto next = node_at(index);
        auto prev = next->prev.lock();
        auto node = std::make_shared<NodeT>(std::move(value));

        node->next = next;
        node->prev = prev;
        if (prev) {
            prev->next = node;
        }
        next->prev = node;
        ++size_;
    }

    void erase(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("DoublyLinkedList::erase: index out of range");
        }
        if (index == 0) {
            pop_front();
            return;
        }
        if (index == size_ - 1) {
            pop_back();
            return;
        }

        auto cur = node_at(index);
        auto prev = cur->prev.lock();
        auto next = cur->next;

        if (prev) {
            prev->next = next;
        }
        if (next) {
            next->prev = prev;
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
        if (head_) {
            head_->prev = node;
        } else {
            tail_ = node;
        }
        head_ = std::move(node);
        ++size_;
    }

    void push_back(T value) {
        auto node = std::make_shared<NodeT>(std::move(value));
        node->prev = tail_;
        if (tail_) {
            tail_->next = node;
        } else {
            head_ = node;
        }
        tail_ = std::move(node);
        ++size_;
    }

    void pop_front() {
        if (empty()) {
            throw std::out_of_range("DoublyLinkedList::pop_front: list is empty");
        }
        head_ = head_->next;
        --size_;
        if (head_) {
            head_->prev.reset();
        } else {
            tail_.reset();
        }
    }

    void pop_back() {
        if (empty()) {
            throw std::out_of_range("DoublyLinkedList::pop_back: list is empty");
        }

        auto prev = tail_->prev.lock();
        if (!prev) {
            head_.reset();
            tail_.reset();
            size_ = 0;
            return;
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
    

    friend std::ostream& operator<<(std::ostream& os, const DoublyLinkedList& list) {
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