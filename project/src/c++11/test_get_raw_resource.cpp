#include <iostream>
using namespace std;

class A {
	public:
		void f();
};

void A::f() {
	cout<<"A::f()"<<endl;
}

class B {
	private:
		A a;
	public:
		B(const A& a):a(a){}
		void f();

		operator A();


};

B::operator A(){
	return a;
}

void B::f() {
	cout<<"B::f()"<<endl;
}

void test(A a) {
	a.f();
}

int main()
{
	A a;
	B b(a);
	auto bb = b ;
	bb.f();
	test(b);
	return 0;
}
