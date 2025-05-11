#include <iostream>

template <typename T> class node;
template <typename T> class my_binary_tree;
template <typename T> class my_red_black_tree;
template <typename T> class my_set;

enum Color { BLACK, RED };

template <typename T> class node {
public:
    node<T> *parent;
    node<T> *left;
    node<T> *right;
    T data;
    Color color;

    node(node<T> *_parent, node<T> *_left, node<T> *_right, T _data, Color _color = BLACK)
        : parent(_parent), left(_left), right(_right), data(_data), color(_color) {};
};

template <typename T> class my_binary_tree {
protected:
    node<T> *root;
private:
    void print_tree_helper(node<T>* current) {
        if (current == nullptr) return;
        print_tree_helper(current->left);
        std::cout << current->data << " ";
        print_tree_helper(current->right);
    }

    void print_tree_visual_helper(node<T>* current, int space = 0) {
        if (current == nullptr) return;
        
        space += 10;
        
        print_tree_visual_helper(current->right, space);
        
        std::cout << std::endl;
        for (int i = 10; i < space; i++)
            std::cout << " ";
        std::cout << current->data;
        if (current->color == RED)
            std::cout << "(R)";
        else
            std::cout << "(B)";
        
        print_tree_visual_helper(current->left, space);
    }

public:
    my_binary_tree<T>(node<T> *_root) : root(_root) {};
    virtual void insert(T value);
    virtual void remove(T value);
    node<T> *search(T value);
    void print_tree();
    void print_tree_visual() {
        print_tree_visual_helper(root);
        std::cout << std::endl;
    }
};

template <typename T> class my_set {
private:
    my_red_black_tree<T> tree;

public:
    void insert(T value);
    bool contains(T value);
    void remove(T value);
    void print();
    void print_tree() { tree.print_tree(); }
    void print_tree_visual() { tree.print_tree_visual(); }
};

template <typename T> void my_set<T>::insert(T value) {
    tree.insert(value);
}

template <typename T> bool my_set<T>::contains(T value) {
    return tree.search(value) != nullptr;
}

template <typename T> void my_set<T>::remove(T value) {
    tree.remove(value);
}

template <typename T> void my_set<T>::print() {
    tree.print_tree();
}

template <typename T> class my_red_black_tree : public my_binary_tree<T> {
private:
    void left_rotate(node<T>* x);
    void right_rotate(node<T>* x);
    void fix_insert(node<T>* k);
    void fix_delete(node<T>* x);
    node<T>* get_successor(node<T>* n);
    void transplant(node<T>* u, node<T>* v);

public:
    my_red_black_tree() : my_binary_tree<T>(nullptr) {}
    void insert(T value) override;
    void remove(T value) override;
};

template <typename T> void my_red_black_tree<T>::left_rotate(node<T>* x) {
    if (x == nullptr || x->right == nullptr) return;
    
    node<T>* y = x->right;
    x->right = y->left;
    
    if (y->left != nullptr) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == nullptr) {
        this->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

template <typename T> void my_red_black_tree<T>::right_rotate(node<T>* x) {
    if (x == nullptr || x->left == nullptr) return;
    
    node<T>* y = x->left;
    x->left = y->right;
    
    if (y->right != nullptr) {
        y->right->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == nullptr) {
        this->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    
    y->right = x;
    x->parent = y;
}

template <typename T> void my_red_black_tree<T>::fix_insert(node<T>* k) {
    node<T>* u;
    while (k->parent != nullptr && k->parent->color == RED) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;
            if (u != nullptr && u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    right_rotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                left_rotate(k->parent->parent);
            }
        } else {
            u = k->parent->parent->right;
            if (u != nullptr && u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    left_rotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                right_rotate(k->parent->parent);
            }
        }
        if (k == this->root) {
            break;
        }
    }
    this->root->color = BLACK;
}

template <typename T> node<T>* my_red_black_tree<T>::get_successor(node<T>* n) {
    node<T>* curr = n->right;
    while (curr && curr->left) {
        curr = curr->left;
    }
    return curr;
}

template <typename T> void my_red_black_tree<T>::transplant(node<T>* u, node<T>* v) {
    if (u->parent == nullptr) {
        this->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

template <typename T> void my_red_black_tree<T>::fix_delete(node<T>* x) {
    while (x != this->root && (x == nullptr || x->color == BLACK)) {
        if (x == nullptr || x == x->parent->left) {
            node<T>* w = x ? x->parent->right : nullptr;
            if (!x) x = nullptr;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(x->parent);
                w = x->parent->right;
            }
            if (w && (!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w && (!w->right || w->right->color == BLACK)) {
                    if (w->left) w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(w);
                    w = x->parent->right;
                }
                if (w) {
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right) w->right->color = BLACK;
                    left_rotate(x->parent);
                }
                x = this->root;
            }
        } else {
            node<T>* w = x->parent->left;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(x->parent);
                w = x->parent->left;
            }
            if (w && (!w->right || w->right->color == BLACK) && (!w->left || w->left->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w && (!w->left || w->left->color == BLACK)) {
                    if (w->right) w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(w);
                    w = x->parent->left;
                }
                if (w) {
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left) w->left->color = BLACK;
                    right_rotate(x->parent);
                }
                x = this->root;
            }
        }
    }
    if (x) x->color = BLACK;
}

template <typename T> void my_red_black_tree<T>::remove(T value) {
    node<T>* z = this->search(value);
    if (z == nullptr) {
        std::cout << value << " not found\n";
        return;
    }

    node<T>* y = z;
    node<T>* x;
    Color y_original_color = y->color;

    if (z->left == nullptr) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == nullptr) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = get_successor(z);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x) x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if (y_original_color == BLACK) {
        fix_delete(x);
    }

    if (this->root) {
        this->root->color = BLACK;
    }
}

template <typename T> void my_red_black_tree<T>::insert(T value) {
    if (this->root == nullptr) {
        this->root = new node<T>(nullptr, nullptr, nullptr, value, BLACK);
        return;
    }

    node<T>* curr = this->root;
    node<T>* parent = nullptr;
    bool isLeft = false;

    while (curr != nullptr) {
        parent = curr;
        if (value < curr->data) {
            curr = curr->left;
            isLeft = true;
        } else if (value > curr->data) {
            curr = curr->right;
            isLeft = false;
        } else {
            return;
        }
    }

    node<T>* newNode = new node<T>(parent, nullptr, nullptr, value, RED);
    if (isLeft) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    fix_insert(newNode);
}

template <typename T> void my_binary_tree<T>::remove(T value) {
    node<T> *curr = root;
    bool left = false;
    
    while (curr != nullptr && curr->data != value) {
        if (curr->data > value) {
            curr = curr->left;
            left = true;
        } else {
            curr = curr->right;
            left = false;
        }
    }
    
    if (curr == nullptr) {
        std::cout << value << " not found\n";
        return;
    }
    
    if (curr->left == nullptr && curr->right == nullptr) {
        if (curr == root) {
            root = nullptr;
        } else if (left) {
            curr->parent->left = nullptr;
        } else {
            curr->parent->right = nullptr;
        }
        delete curr;
    }
    else if (curr->left == nullptr || curr->right == nullptr) {
        node<T> *child = (curr->left != nullptr) ? curr->left : curr->right;
        
        if (curr == root) {
            root = child;
            child->parent = nullptr;
        } else {
            child->parent = curr->parent;
            if (left) {
                curr->parent->left = child;
            } else {
                curr->parent->right = child;
            }
        }
        delete curr;
    }
    else {
        node<T> *successor = curr->right;
        while (successor->left != nullptr) {
            successor = successor->left;
        }
        
        T successorValue = successor->data;
        remove(successorValue);
        curr->data = successorValue;
    }
}

template <typename T> void my_binary_tree<T>::insert(T value) {
    if (root == nullptr) {
        root = new node<T>(nullptr, nullptr, nullptr, value);
        return;
    }

    node<T> *curr = root;
    while (true) {
        if (value < curr->data) {
            if (curr->left == nullptr) {
                curr->left = new node<T>(curr, nullptr, nullptr, value);
                break;
            }
            curr = curr->left;
        } else if (value > curr->data) {
            if (curr->right == nullptr) {
                curr->right = new node<T>(curr, nullptr, nullptr, value);
                break;
            }
            curr = curr->right;
        } else {
            return;
        }
    }
}

template <typename T> node<T> *my_binary_tree<T>::search(T value) {
    node<T> *curr = root;
    while (curr != nullptr) {
        if (curr->data == value) {
            return curr;
        }
        if (curr->data > value) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return nullptr;
}

template <typename T> void my_binary_tree<T>::print_tree() {
    if (!root) return;
    print_tree_helper(root);
    std::cout << std::endl;
}

int main() {
    my_set<int> s;
    
    s.insert(50);
    s.insert(30);
    s.insert(70);
    s.insert(20);
    s.insert(40);
    s.insert(10);
    s.insert(60);
    s.insert(80);
    s.insert(15);
    s.insert(35);
    s.insert(45);
    s.insert(55);
    s.insert(75);
    s.insert(85);
    s.insert(90);
    
    std::cout << "\n" << "Original tree ";
    s.print();
    
    std::cout << "\nVisual tree:\n";
    s.print_tree_visual();
    
    std::cout << "Contains 40? -> " << s.contains(40) << std::endl;
    std::cout << "Contains 25? -> " << s.contains(25) << std::endl;
    
    s.remove(30);
    
    std::cout << "\nDeleted 30: ";
    s.print();
    
    std::cout << "\nNew structure:\n";
    s.print_tree_visual();
    
    return 0;
}