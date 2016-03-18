#ifndef _SINGLETON_
#define _SINGLETON_

namespace Common
{

namespace Base
{
	/*
	 * ���� --- ����ģʽ
	 * ���̰߳�ȫ
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
