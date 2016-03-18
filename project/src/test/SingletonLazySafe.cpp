#include <iostream>
using namespace std;

/*
 * 单例模式 ---- 懒汉模式 
 * 解决了内存泄露、多线程问题
 */

class SingletonLazySafe{
	private:
		SingletonLazySafe(){}

		class DelSingleton {
			public:
				~DelSingleton() {
					if ( SingletonLazySafe::m_instance )
						delete SingletonLazySafe::m_instance;
				}
		};
	
		static DelSingleton m_del;

		static SingletonLazySafe * m_instance;

	public:

		static SingletonLazySafe * getInstance() {
			
			if (m_instance == NULL) {
				// Lock() TODO 这里加锁
				if (m_instance == NULL)
					m_instance = new SingletonLazySafe();
				// UnLock() TODO 这里解锁
			}
			return m_instance;
		}

		static SingletonLazySafe * getPoint() {
			return SingletonLazySafe::m_instance;
		}
};

SingletonLazySafe * SingletonLazySafe::m_instance;

int main(){
	cout << (SingletonLazySafe::getPoint() == nullptr) << endl;
	SingletonLazySafe * instance = SingletonLazySafe::getInstance();
	return 0;
}
