#include <iostream>
using namespace std;

/*
 * 单例模式 ---- 懒汉模式
 * 缺点:指针可能未被释放,造成内存泄露
 */

class SingletonLazy{
	private:
		SingletonLazy(){}
	
	public:
		static SingletonLazy * m_instance;

		static SingletonLazy * getInstance() {
			if (m_instance == NULL)
				m_instance = new SingletonLazy();
			return m_instance;
		}
};

SingletonLazy * SingletonLazy::m_instance;

int main(){
	cout << (SingletonLazy::m_instance == NULL) << endl;
	SingletonLazy * instance = SingletonLazy::getInstance();
	return 0;
}
