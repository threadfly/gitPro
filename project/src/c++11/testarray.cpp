#include <iostream>
#include <array>
using namespace std;


int main() {
	array<int, 4> arr = {1,2,3,4};
	cout << "------------" << endl;
	for (auto it = begin(arr); it != end(arr); it++){
		cout << *it << endl;
	}
	cout << "------------" << endl;
	return 0;
}
