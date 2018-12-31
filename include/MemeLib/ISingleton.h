#ifndef _ISINGLETON_H_
#define _ISINGLETON_H_

#include <MemeLib/INonCopyable.h>

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

#endif // !_ISINGLETON_H_
