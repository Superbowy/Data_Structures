#include <cstdint>
#include <iostream>
#include <stdexcept>

class my_array {

  public:
	my_array(unsigned int size);
	~my_array();
	void print();

	int get(int index);
	void set(int index, int value);
	size_t size();
	size_t capacity();

  private:
	size_t _size;
	size_t _capacity;

	void *base_address;
};

int main() {
	my_array declared = my_array(10);

	declared.set(0, 3);
	declared.set(4, 17);
	declared.set(9, 2);

    std::cout << "Array declared with capacity " << declared.capacity() << " and current length " << declared.size()
	          << "\n";

	declared.print();

	return 0;
}

my_array::my_array(unsigned int __capacity) {

	_capacity = __capacity;
	_size = 0;

	base_address = std::malloc(_capacity * sizeof(int));
}

my_array::~my_array() { std::free(base_address); }

int my_array::get(int index) {

	if (index > _capacity || index < 0)
		throw std::out_of_range("Index out of bounds !");

	std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(base_address);
	int *target = reinterpret_cast<int *>(addr + index * sizeof(int));

	return *target;
}
void my_array::set(int index, int value) {

	if (index > _capacity || index < 0)
		throw std::out_of_range("Index out of bounds !");

	std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(base_address);
	int *target = reinterpret_cast<int *>(addr + index * sizeof(int));

	*target = value;
    _size++;
}

void my_array::print() {

    std::cout << "| ";

	for (int i = 0; i < _capacity; i++) {
		std::cout << this->get(i) << " | ";
	}
}

size_t my_array::size() { return _size; }
size_t my_array::capacity() { return _capacity; }
