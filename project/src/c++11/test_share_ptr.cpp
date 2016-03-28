#include <iostream>
#include <memory>

using namespace std;

class Mutex {
	public:
	void Lock() {
		cout << "Lock" << endl;
	}
	void UnLock() {
		cout << "UnLock" << endl;
	}

	~Mutex() {
		cout << "dtr" << endl;
	}

};

class Locker {
	public:
	Locker(Mutex * mutex):_mutex_sptr(mutex,
			[](Mutex* ptr){
				ptr->UnLock();
			})
	{
		Lock();
	}

	void Lock() {
		_mutex_sptr->Lock();
	}

	void UnLock() {
		_mutex_sptr->UnLock();
	}

	private:

	shared_ptr<Mutex> _mutex_sptr;
};

int main()
{
	Mutex mtx;
	{
		Locker lke1(&mtx);
		Locker lke2(lke1);
	}
	return 0;
}
