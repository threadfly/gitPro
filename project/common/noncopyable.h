#ifndef _NONCOPYABLE_
#define _NONCOPYABLE_

namespace Common{

namespace Noncopy{

class NonCopyable {
	protected:
		/*
		 * 
		 */
		NonCopyable(){}
		~NonCopyable(){}
	private:
		/*
		 * 禁用拷贝构造函数
		 */
		NonCopyable(const NonCopyable &);//{}

		/*
		 * 禁用赋值构造函数
		 */
		const NonCopyable& operator=(const NonCopyable &);
};
/*
const NonCopyable & NonCopyable::operator=(const NonCopyable &)
{
	return *this;
}
*/

}

}// namespace Common 

#endif
