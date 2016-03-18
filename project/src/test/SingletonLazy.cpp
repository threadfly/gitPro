#include <iostream>
using namespace std;

/*
 * ����ģʽ ---- ����ģʽ
 * ȱ��:ָ�����δ���ͷ�,����ڴ�й¶
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
