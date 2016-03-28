#include <iostream>
using namespace std;

class A {
	public:
		A(){}
	private:
		size_t c;
};

class B: public A {
	private:
		size_t c;
	public:
		size_t f() {
			return c;
		}
};


int main()
{
	B b;
	cout << b.f() << endl;
	return 0;
}
