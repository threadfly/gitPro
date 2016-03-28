#include <iostream>

#include <memory>

class Buffer {
	char* _data;
	size_t _size;

	public:
	Buffer(size_t );
}

Buffer::Buffer(size_t size) {
	if ( size == 0 )
		size == 1;
	
	_data = new char[size] ();
	_size = size;
}

int main() {
	auto b_sptr = std::make_shared<Buffer> (10);
	return 0;
}
