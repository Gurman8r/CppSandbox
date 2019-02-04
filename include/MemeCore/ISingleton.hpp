#ifndef _I_SINGLETON_HPP_
#define _I_SINGLETON_HPP_

#include <MemeCore/INonCopyable.hpp>

namespace ml
{
	template <class T>
	class ISingleton
		: public INonCopyable
	{
	public:
		inline static T & getInstance()
		{
			static T instance;
			return instance;
		}
	};
}

#endif // !_I_SINGLETON_HPP_
