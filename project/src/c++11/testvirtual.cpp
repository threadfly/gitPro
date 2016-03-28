#include <iostream>
using namespace std;

class A {
	public: A(){};

	virtual int f() = 0;

	virtual ~A() = 0;
};

A::~A(){}

//int A::f(){return 0;}

class B : public A{
	int f();
};

int B::f() {return 0;}

int main()
{
	A* b = new B;
	cout << b->f() << endl;
	delete b;
	return 0;
}
