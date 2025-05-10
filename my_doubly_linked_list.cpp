#include <iostream>

template <typename T>
class node {
public:
    node<T>* previous;
    node<T>* next;
    T data;
    node(node<T>* _previous, node<T>* _next, T _data)
        : previous(_previous), next(_next), data(_data) {}
};

template <typename T>
class my_doubly_linked_list {
private:
    node<T>* root;
    node<T>* end;
    size_t _size;

public:
    my_doubly_linked_list() : root(nullptr), end(nullptr), _size(0) {}

    ~my_doubly_linked_list() {
        node<T>* current = root;
        while (current != nullptr) {
            node<T>* next = current->next;
            delete current;
            current = next;
        }
    }

    void push_front(T data) {
        node<T>* new_node = new node<T>(nullptr, root, data);
        if (root != nullptr) root->previous = new_node;
        root = new_node;
        if (_size == 0) end = root;
        _size++;
    }

    void push_back(T data) {
        node<T>* new_node = new node<T>(end, nullptr, data);
        if (end != nullptr) end->next = new_node;
        end = new_node;
        if (_size == 0) root = end;
        _size++;
    }

    void insert(node<T>* pos, T data) {
        if (pos == nullptr) return;
        node<T>* new_node = new node<T>(pos->previous, pos, data);
        if (pos->previous != nullptr) {
            pos->previous->next = new_node;
        } else {
            root = new_node;
        }
        pos->previous = new_node;
        _size++;
    }

    void erase(node<T>* pos) {
        if (pos == nullptr) return;
        if (pos->previous != nullptr)
            pos->previous->next = pos->next;
        else
            root = pos->next;

        if (pos->next != nullptr)
            pos->next->previous = pos->previous;
        else
            end = pos->previous;

        delete pos;
        _size--;
    }

    void swap(node<T>* a, node<T>* b) {
        if (a == nullptr || b == nullptr || a == b) return;
        T temp = a->data;
        a->data = b->data;
        b->data = temp;
    }

    void print_list() {
        node<T>* curr = root;
        while (curr != nullptr) {
            std::cout << curr->data << (curr->next ? " <-> " : "");
            curr = curr->next;
        }
        std::cout << std::endl;
    }

    node<T>* head() { return root; }
    node<T>* tail() { return end; }
    size_t size() { return _size; }
};

int main() {
    my_doubly_linked_list<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_front(5);
    list.push_back(30);
    list.print_list();
    node<int>* second = list.head()->next;
    list.insert(second, 15);

    list.print_list();

    list.swap(second, second->next);
    list.print_list();

    list.erase(second);
    list.print_list();

    return 0;
}

