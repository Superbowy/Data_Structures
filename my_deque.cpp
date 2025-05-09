// This implementation uses a doubly-linked list. Dynamic arrays can also be used.
#include <iostream>
template <typename T> class node {
  public:
	node<T> *previous;
	node<T> *next;
	T data;
	node(node<T> *_previous, node<T> *_next, T _data) : previous(_previous), next(_next), data(_data) {}
};
template <typename T> class my_deque {
  private:
	size_t _size;
	node<T> *root;
	node<T> *end;

  public:
	my_deque(T init_data) {
		root = new node<T>(nullptr, nullptr, init_data);
		_size = 1;
		end = root;
	};
	void push_front(T data);
	void push_back(T data);
	void pop_front();
	void pop_back();
	node<T> *front();
	node<T> *back();
	size_t size();
	void print_deque();
};
template <typename T> void my_deque<T>::push_front(T data) {
	node<T> *old_root = root;
	root = new node<T>(nullptr, old_root, data);
	if (_size == 0) { // handle empty deque
		end = root;
	} else {
		old_root->previous = root;
	}
	_size++;
};
template <typename T> void my_deque<T>::push_back(T data) {
	node<T> *old_end = end;
	end = new node<T>(old_end, nullptr, data);
	if (_size == 0) { // handle empty deque
		root = end;
	} else {
		old_end->next = end;
	}
	_size++;
};
template <typename T> void my_deque<T>::pop_front() {
	if (_size > 0) { // check if the deque is empty
		node<T> *first = root;
		root = root->next;
		if (root != nullptr) { // check if new root is null
			root->previous = nullptr;
		} else {
			end = nullptr; // if new root is null, end should also be null
		}
		_size--;
		delete first;
	}
};
template <typename T> void my_deque<T>::pop_back() {
	if (_size > 0) { // check if the deque is empty
		node<T> *last = end;
		end = end->previous;
		if (end != nullptr) { // check if new end is null
			end->next = nullptr;
		} else {
			root = nullptr; // if new end is null, root should also be null
		}
		_size--;
		delete last;
	}
};
template <typename T> node<T> *my_deque<T>::front() { return root; };
template <typename T> node<T> *my_deque<T>::back() { return end; };
template <typename T> size_t my_deque<T>::size() { return _size; }
template <typename T> void my_deque<T>::print_deque() {
	node<T> *curr = root;
	while (curr != nullptr) {
		std::cout << curr->data << (curr->next != nullptr ? " -> " : "");
		curr = curr->next;
	}
	std::cout << std::endl;
};
int main() {
	my_deque<int> deque = my_deque<int>(12);
	deque.push_front(5);
	deque.push_back(20);
	deque.push_front(3);
	deque.push_back(25);
	deque.push_front(1);
	deque.print_deque();
	return 0;
}
