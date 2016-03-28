#include <iostream>
#include <vector>
using namespace std;


int main()
{
	int arr[] = {1,2,3,4};
	vector<int> vec(arr, arr+4);
	vector<int>::iterator b = vec.begin();
	vector<int>::iterator e = vec.end();
	auto diff = b - e;
	cout << diff << endl;
	return 0 ;
}
