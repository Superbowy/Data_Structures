#include <cstddef>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

class my_unordered_map {

  private:
	struct Node {
		std::string key;
		int value;
		Node *next;
		Node(std::string k, int v, Node *n) : key(k), value(v), next(n) {}
	};
	Node **buckets;
	size_t _size;
	size_t _count;

	void rehash();

  public:
	my_unordered_map();
	~my_unordered_map();
	void insert(std::string key, int value);
	void erase(std::string key);
	int find(std::string key);
	void clear();
	size_t size();
};

my_unordered_map::my_unordered_map() : _size(10), _count(0) {
	buckets = new Node*[_size]();
}

my_unordered_map::~my_unordered_map() {
	clear();
	delete[] buckets;
}

void my_unordered_map::rehash() {
	_size *= 2;  // Double the size of the bucket array
	Node **new_buckets = new Node*[_size]();

	for (size_t i = 0; i < _size / 2; ++i) {
		Node *curr = buckets[i];
		while (curr != nullptr) {
			unsigned int new_bucket_index = std::hash<std::string>{}(curr->key) % _size;
			Node *next_node = curr->next;
			curr->next = new_buckets[new_bucket_index];
			new_buckets[new_bucket_index] = curr;
			curr = next_node;
		}
	}

	delete[] buckets;
	buckets = new_buckets;
    std::cout << "Re-hash done\n";
}

void my_unordered_map::insert(std::string key, int value) {
	// Check load factor before inserting
	if (_count >= _size * 0.75) {
		rehash();
	}

	unsigned int bucket_index = std::hash<std::string>{}(key) % _size;

	if (buckets[bucket_index] == nullptr) {
		buckets[bucket_index] = new Node(key, value, nullptr);
	} else {
		Node *curr = buckets[bucket_index];
		while (curr->next != nullptr) {
			curr = curr->next;
		}
		curr->next = new Node(key, value, nullptr);
	}
	++_count;
}

int my_unordered_map::find(std::string key) {
	unsigned int bucket_index = std::hash<std::string>{}(key) % _size;

	if (buckets[bucket_index] == nullptr) {
		throw std::runtime_error("No value found with this key");
	} else {
		Node *curr = buckets[bucket_index];
		while (curr != nullptr) {
			if (curr->key == key) {
				return curr->value;
			}
			curr = curr->next;
		}
		throw std::runtime_error("No value found with this key");
	}
}

void my_unordered_map::erase(std::string key) {
	unsigned int bucket_index = std::hash<std::string>{}(key) % _size;

	Node *curr = buckets[bucket_index];
	Node *prev = nullptr;

	while (curr != nullptr) {
		if (curr->key == key) {
			if (prev == nullptr) {
				buckets[bucket_index] = curr->next;
			} else {
				prev->next = curr->next;
			}
			delete curr;
			--_count;
			return;
		}
		prev = curr;
		curr = curr->next;
	}
	throw std::runtime_error("No value found with this key to erase");
}

void my_unordered_map::clear() {
	for (size_t i = 0; i < _size; ++i) {
		Node *curr = buckets[i];
		while (curr != nullptr) {
			Node *to_delete = curr;
			curr = curr->next;
			delete to_delete;
		}
		buckets[i] = nullptr;
	}
	_count = 0;
}

size_t my_unordered_map::size() {
	return _count;
}

int main() {
	my_unordered_map my_map;

	my_map.insert("A", 1);
	my_map.insert("B", 3);
	my_map.insert("C", 8);
	my_map.insert("F", 9);
	my_map.insert("J", 2);
	my_map.insert("N", 0);
	my_map.insert("Y", 4);
	my_map.insert("T", 4);
	my_map.insert("O", 4);
	my_map.insert("Z", 4);

	std::cout << my_map.find("Z") << "\n";

	my_map.erase("C");

	try {
		std::cout << my_map.find("C") << "\n";
	} catch (const std::runtime_error &e) {
		std::cout << e.what() << "\n";
	}

	std::cout << "Current size : " << my_map.size() << "\n";

	my_map.clear();

	std::cout << "Current size after clear: " << my_map.size() << "\n";

	return 0;
}

