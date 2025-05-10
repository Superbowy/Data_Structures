#include <iostream>

template <typename T> class node {

  public:
	node<T> *parent;
	node<T> *left;
	node<T> *right;
	T data;

	node(node<T> *_parent, node<T> *_left, node<T> *_right, T _data)
	    : parent(_parent), left(_left), right(_right), data(_data) {};
};

template <typename T> class my_binary_tree {
  private:
	node<T> *root;
    void print_tree_helper(node<T>* current) {
        if (current == nullptr) return;
        print_tree_helper(current->left);
        std::cout << current->data << " ";
        print_tree_helper(current->right);
    }

  public:
	my_binary_tree<T>(node<T> *_root) : root(_root){};
	void insert(T value);
	void remove(T value);
	node<T> *search(T value);
	void print_tree();
};

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
	node<T> *curr = root;
	while (true) {
		if (curr->data == value) {
			std::cout << "Duplicate: " << value << "\n";
			return;
		}
		if (curr->data > value) {
			if (curr->left == nullptr) {
				curr->left = new node<T>(curr, nullptr, nullptr, value);
				break;
			} else {
				curr = curr->left;
			}
		} else {
			if (curr->right == nullptr) {
				curr->right = new node<T>(curr, nullptr, nullptr, value);
				break;
			} else {
				curr = curr->right;
			}
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
	node<int> *node1 = new node<int>(nullptr, nullptr, nullptr, 10);
	my_binary_tree<int> tree1 = my_binary_tree<int>(node1);

	tree1.insert(8);
	tree1.insert(2);
	tree1.insert(29);
	tree1.insert(14);
	tree1.insert(2);
	tree1.insert(7);
    tree1.insert(1);
	tree1.remove(2);

    int value1 = 14;
    int value2 = 99;

    node<int> *result1 = tree1.search(value1);
    if (result1 != nullptr) {
        std::cout << value1 << " found\n";
    } else {
        std::cout << value1 << " not found\n";
    }

    node<int> *result2 = tree1.search(value2);
    if (result2 != nullptr) {
        std::cout << value2 << " found\n";
    } else {
        std::cout << value2 << " not found\n";
    }

	tree1.print_tree();
	return 0;
}
