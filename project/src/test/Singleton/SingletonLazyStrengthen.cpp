#include <iostream>
using namespace std;

/*
 * 单例模式 ---- 懒汉模式 解决之前因指针未释放导致内存泄露问题
 * 缺点: 多线程不安全
 */

class SingletonLazyStrengthen{
	private:
		SingletonLazyStrengthen(){}

		class DelSingleton {
			public:
				~DelSingleton() {
					if ( SingletonLazyStrengthen::m_instance )
						delete SingletonLazyStrengthen::m_instance;
				}
		};
	
		static DelSingleton m_del;

		static SingletonLazyStrengthen * m_instance;

	public:

		static SingletonLazyStrengthen * getInstance() {
			if (m_instance == NULL)
				m_instance = new SingletonLazyStrengthen();
			return m_instance;
		}

		static SingletonLazyStrengthen * getPoint() {
			return SingletonLazyStrengthen::m_instance;
		}
};

SingletonLazyStrengthen * SingletonLazyStrengthen::m_instance = new SingletonLazyStrengthen;
//SingletonLazyStrengthen * SingletonLazyStrengthen::m_instance;  // hungry model

int main(){
	cout << (SingletonLazyStrengthen::getPoint() == nullptr) << endl;
	SingletonLazyStrengthen * instance = SingletonLazyStrengthen::getInstance();
	return 0;
}
