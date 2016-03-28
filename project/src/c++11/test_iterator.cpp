#include <iostream>
#include <vector>
using namespace std;

int main()
{
	vector<int> vec2 = {1,2,3,4};
	auto it = --vec2.cend();
	vector<int> vec(vec2.cbegin(), it);
	for (auto &v : vec) {
		v++;
	}

	for ( auto v : vec) {
		cout << v << endl;
	}

	return 0;
}
