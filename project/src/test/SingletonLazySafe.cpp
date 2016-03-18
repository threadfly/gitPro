#include <iostream>
using namespace std;

/*
 * ����ģʽ ---- ����ģʽ 
 * ������ڴ�й¶�����߳�����
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
				// Lock() TODO �������
				if (m_instance == NULL)
					m_instance = new SingletonLazySafe();
				// UnLock() TODO �������
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
