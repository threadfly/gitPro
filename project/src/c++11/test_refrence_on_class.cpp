#include <iostream>
using namespace std;

class A {
	public:
		~A() {
			cout << "dector" << endl;
		}

		void x() {
			cout << "xxx" << endl ;
		}
};

class B { 
	private:
		A & a;

	public:
		B(A& a):a(a){
		}
	
		void f() {
			a.x();
		}
};


int main()
{
	A *a = new A;
	B b(*a);
	{
		B c(b);
		c.f();
	}
	return 0;
}
