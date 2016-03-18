#include <iostream>
using namespace std;

/*
 * 单例模式 ---- 内部静态实例实现的懒汉汉模式
 * c++0x后，编译器保证内部静态变量的线程安全性
 */

class SingletonLazyInner{
	private:
		SingletonLazyInner(){}
	
	public:

		static SingletonLazyInner * getInstance() {
			// Lock()		// not needed after c++0x
			static SingletonLazyInner m_instance;
			// UnLock()		// not neede after c++0x
			return &m_instance;
		}
};

int main(){
	SingletonLazyInner * instance = SingletonLazyInner::getInstance();
	return 0;
}
