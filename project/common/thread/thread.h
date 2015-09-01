#ifndef _THREAD_
#define _THREAD_
#include "noncopyable.h"
#include "pthread.h"

namespace Common {

namespace Thread {

		using namespace Common::Noncopy;
		class Thread: public NonCopyable
		{
		public:
			/*
			 * name: 构造函数
			 * param: isDetach 决定线程是否是可以Detach的 
			 */
			Thread(bool isDetach);
			virtual ~Thread();
			/*
			 * 线程启动
			 */
			int Start();
			
			/*
			 * 是不是当前线程
			 */
			bool IsSelf();

			/*
			 * 继承此类的派生类应实现此方法
			 */
			virtual void Run() = 0;

			static void *  Loop(void * args){ 
				Thread* pthread = (Thread*)args;
				if (NULL == pthread)
				{
					// TODO  report error
				} else 
				{
					pthread->Run();	
				}

				return NULL;
			}

			bool Join();
		private:
			pthread_t		m_pid;
			pthread_attr_t	m_attr;
			bool			m_isDetach;
		};

} // namespace Thread

} // namespace Common
#endif
