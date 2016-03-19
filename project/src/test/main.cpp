#include <iostream>
#include "singleton.h"

using namespace std;

using namespace Common::Base;
using namespace Common::Noncopy;

class A: public NonCopyable {
	friend class SingletonLazy<A>;
	friend class SingletonEager<A>;
	private:
		A(){}
	public:
		void print() {
			cout << "I am A"<<endl;
		}

};

int main()
{
	A &a = SingletonLazy<A>::getInstance();
	a.print();
	cout << (&a == &SingletonLazy<A>::getInstance())<<endl;
	//*a = *SingletonLazy<A>::getInstance();
	//delete a;
	SingletonEager<A>::getInstance()->print();
	return 0;
}
