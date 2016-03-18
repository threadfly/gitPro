#include <iostream>
using namespace std;

/*
 * 单例模式 ---- 饿汉模式
 * 性能高，避免竞争
 */

class SingletonHungry{
	private:
		SingletonHungry(){}
		
		int m_num;
	public:
		static SingletonHungry * const  m_instance;  // const pointer, non-const data

		static SingletonHungry * const getInstance() {
			return m_instance;
		}

		void setNum(int m) {
			m_num = m;
		}
};

SingletonHungry * const SingletonHungry::m_instance = new SingletonHungry;

int main(){
	cout << (SingletonHungry::m_instance == NULL) << endl;
	SingletonHungry * const instance = SingletonHungry::getInstance();
	instance->setNum(1);
	return 0;
}
