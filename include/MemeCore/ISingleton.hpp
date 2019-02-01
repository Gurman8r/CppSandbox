#ifndef _ISINGLETON_HPP_
#define _ISINGLETON_HPP_

#include <MemeCore/INonCopyable.hpp>

namespace ml
{
	template <typename T>
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

#endif // !_ISINGLETON_HPP_
