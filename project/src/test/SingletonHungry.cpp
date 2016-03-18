#include <iostream>
using namespace std;

/*
 * 单例模式 ---- 饿汉模式
 */

class SingletonHungry{
	private:
		SingletonHungry(){}
	
	public:

		static SingletonHungry * getInstance() {
			static SingletonHungry m_instance;
			return &m_instance;
		}
};

int main(){
	SingletonHungry * instance = SingletonHungry::getInstance();
	return 0;
}
