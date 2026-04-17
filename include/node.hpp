#pragma once

#include <memory>
#include <utility>

template <typename T>
struct Node {
    T value;
    std::shared_ptr<Node<T>> next{};
    std::weak_ptr<Node<T>> prev{};

    explicit Node(T v) : value(std::move(v)) {}
};