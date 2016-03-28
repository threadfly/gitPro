#include <iostream>
using namespace std;

class A {
	public:
	A()
	{
		f();
	}

	virtual int f()
	{
		cout << "AAA" << endl;
		return 0;
	}
};

class B: public A{
	public:
	int f()
	{
		cout << "BBB" << endl;
		return 0;
	}
};

int main() {
	B b;
	cout << "CCC" << endl;
	b.f();
	return 0;
}
