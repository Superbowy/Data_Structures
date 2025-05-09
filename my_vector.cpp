#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>

class my_vector {
  private:
	size_t _capacity;
	size_t _size;
	void *base_address;

  public:
	my_vector(int __capacity = 10);
	~my_vector();
	void print();
	int get(int index);
	void set(int index, int value);
	void push_back(int value);
	size_t size();
	size_t capacity();
};

my_vector::my_vector(int __capacity) {

	_capacity = __capacity;
	_size = 0;

	base_address = std::malloc(_capacity * sizeof(int));
}

my_vector::~my_vector() { std::free(base_address); }

int my_vector::get(int index) {

	if (index > _capacity || index < 0)
		throw std::out_of_range("Index out of bounds !");

	std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(base_address);
	int *target = reinterpret_cast<int *>(addr + index * sizeof(int));

	return *target;
}

void my_vector::set(int index, int value) {
	if (index > _capacity || index < 0)
		throw std::out_of_range("Index out of bounds !");

	std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(base_address);
	int *target = reinterpret_cast<int *>(addr + index * sizeof(int));

	*target = value;
}

void my_vector::push_back(int value) {

	if (_size == _capacity) {
        void* new_base = std::malloc((_capacity + 10) * sizeof(int));
        if (!new_base) throw std::bad_alloc();

        std::uintptr_t old_addr = reinterpret_cast<std::uintptr_t>(base_address);
        std::uintptr_t new_addr = reinterpret_cast<std::uintptr_t>(new_base);

        for (std::size_t i = 0; i < _size; ++i) {
            int* old_elem = reinterpret_cast<int*>(old_addr + i * sizeof(int));
            int* new_elem = reinterpret_cast<int*>(new_addr + i * sizeof(int));
            *new_elem = *old_elem;
        }

        std::free(base_address);
        base_address = new_base;
        _capacity = _capacity + 10;
	}

    this->set(_size, value);
    _size++;
}

void my_vector::print() {

	std::cout << "| ";

	for (int i = 0; i < _capacity; i++) {
		std::cout << this->get(i) << " | ";
	}
}

size_t my_vector::size() { return _size; }
size_t my_vector::capacity() { return _capacity; }

int main() {
	my_vector declared = my_vector(3);

	declared.push_back(4);
	declared.push_back(1298);
	declared.push_back(8);
    declared.push_back(19);
    declared.set(2, 10);

	std::cout << "Vector declared with capacity " << declared.capacity() << " and current length " << declared.size()
	          << "\n";

	declared.print();

	return 0;
}
