#include <iostream>
#include <vector>
using namespace std;

class A {
	public: 
		A(){};

	int f() const {
		cout << "const" << endl;
		return 0;
	}

	int f() {
		cout << "no const" << endl;
		return 0;
	}

};

int main()
{
	const A a;
	cout << a.f() << endl;
	
	vector<int> vec2 = {1,2,4,5};

	int arr[] = {1,2,3,4,5};
	vector<int> vec(arr, arr+5);
	*vec.begin() = 55 ;
	cout << *vec.cbegin() << endl;
	return 0;
}
