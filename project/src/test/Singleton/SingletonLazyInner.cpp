#include <iostream>
using namespace std;

/*
 * ����ģʽ ---- �ڲ���̬ʵ��ʵ�ֵ�������ģʽ
 * c++0x�󣬱�������֤�ڲ���̬�������̰߳�ȫ��
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
