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
		 * ���ÿ������캯��
		 */
		NonCopyable(const NonCopyable &){}

		/*
		 * ���ø�ֵ���캯��
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
