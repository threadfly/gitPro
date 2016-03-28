#include <iostream>
using namespace std;

class A {
	public:
		A(){};

		A(const A &a)
		:c(a.c)
		{
		}
		void SetA(size_t a){ c = a;}
		size_t GetA(){ return c;}
	private:
		size_t c;
};

class B: public A {
	private:
		size_t c;

	public:
		B(){}

		B(const B &b )
		:c(b.c),A(b)
		{
		}

		void SetB(size_t b) { c = b;}
		size_t GetB(){ return c;}
};


int main()
{
	B x1;
	x1.SetA(1), x1.SetB(11);
	B x2(x1);
	cout << x2.GetA() << endl;
	cout << x2.GetB()<< endl;
	return 0;
}
