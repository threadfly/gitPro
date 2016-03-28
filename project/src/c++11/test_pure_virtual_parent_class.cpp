#include <iostream>
using namespace std;

class A {
	public:
		virtual void f()=0;
		virtual ~A()=0;
};

void A::f() {
	cout<<"A::f()"<<endl;
}

A::~A(){}


class B : public A {
	public:
	virtual void f();
};

void B::f() {
	A::f();
}

int main()
{
	B b;
	auto bb = b ;
	bb.f();
	return 0;
}
