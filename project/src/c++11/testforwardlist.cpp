#include <iostream>
#include <forward_list>
using namespace std;


int main() {
	forward_list<int> list = {2,3,4,5};
	for (auto &it : list) {
		it++;
	}
	for (auto &it : list) {
		cout << it<< endl;
	}
	cout << list.max_size()<< endl;
	cout << list.size()<< endl;
	return 0;
}
