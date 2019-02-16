#ifndef _I_READABLE_HPP_
#define _I_READABLE_HPP_

#include <MemeCore/String.hpp>

namespace ml
{
	class IReadable
	{
	public:
		virtual bool cleanup() = 0;
		virtual bool loadFromFile(const String & filename) = 0;
	};
}

#endif // !_I_READABLE_HPP_