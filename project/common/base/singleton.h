#ifndef _SINGLETON_
#define _SINGLETON_

namespace Common
{

namespace Base
{
	/*
	 * 单例 --- 懒汉模式
	 * 非线程安全
	 */

	template <class T>
	class SingletonLazy{
		public:
			T * getInstance() {
				static T instance;
				return &instance;
			}
	};


}

}

#endif
