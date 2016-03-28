#include <iostream>
using namespace std;

class B {
	protected:
		B(){
			cout << "B contructor" << endl;
		}
};

class A:public B {
	private:
		static void print(){
			cout << "A::print" << endl;
		}
		
	public:
		static void f() {
			A::print();
		}

		static A * getInstance() {
			return new A;
		}

		A(){
			cout << "A contructor" << endl;
		}

};

int main() {
	//A::f();
	//A * a = A::getInstance();
	A b;
	return 0;
}
