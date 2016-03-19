#ifndef _SINGLETON_
#define _SINGLETON_

#include "noncopyable.h"

namespace Common
{

namespace Base
{
	/*
	 * ���� --- ����ģʽ
	 * ���̰߳�ȫ
	 */
	using namespace Common::Noncopy;

	template <class T>
	class SingletonLazy {
		public:
			static T& getInstance() {
				static T instance;
				return instance;
			}

	};


	template <class T>
	class SingletonEager{
		public:
			static T * getInstance(){
				return instance;
			}

		private:
			static T * instance;

	};

	template<class T> 
	T * SingletonEager<T>::instance = new T;
}

}

#endif
