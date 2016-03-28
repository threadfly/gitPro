#include <iostream>
#include <array>
using namespace std;

class A {
	private:
	size_t flag;
	public:
	A(size_t n) {
		flag = n;
	}
	static size_t num;

	friend ostream & operator<<(ostream& o, A & a){
		o << a.Flag();
		return o;
	}

	size_t Flag() { return flag;}
};

size_t A::num = 0;

int main()
{
	array<A, 5> arr = {A(1), A(2), A(3), A(4), A(5)};
	array<A, 5> arr2 = {A(6), A(7), A(8), A(9), A(10)};

	auto it = arr.begin();

	arr.swap(arr2);
	cout << *it << endl;
	for ( auto v : arr) {
		cout << v << endl;
	}

	return 0;
}
