#include <iostream>

#include "doubly_linked_list.hpp"
#include "singly_linked_list.hpp"

int main() {
    SinglyLinkedList<int> s;
    s.push_back(1);
    s.push_back(2);
    s.push_front(0);
    std::cout << "Singly: " << s << " (size=" << s.size() << ")\n";

    s.insert(1, 99);
    std::cout << "Singly after insert(1,99): " << s << "\n";
    std::cout << "Singly at(1): " << s.at(1) << "\n";
    std::cout << "Singly index_of(2): " << s.index_of(2) << "\n";

    s.erase(1);
    s.pop_front();
    std::cout << "Singly after erase(1) + pop_front: " << s << "\n";

    DoublyLinkedList<int> d;
    d.push_back(10);
    d.push_back(20);
    d.push_front(5);
    std::cout << "Doubly: " << d << " (size=" << d.size() << ")\n";

    d.insert(2, 15);
    std::cout << "Doubly after insert(2,15): " << d << "\n";
    std::cout << "Doubly at(2): " << d.at(2) << "\n";
    std::cout << "Doubly index_of(20): " << d.index_of(20) << "\n";

    d.erase(1);
    d.pop_back();
    std::cout << "Doubly after erase(1) + pop_back: " << d << "\n";

    return 0;
}
