#ifndef _I_RESOURCE_HPP_
#define _I_RESOURCE_HPP_

#include <MemeCore/String.hpp>

namespace ml
{
	class IResource
	{
	public:
		virtual bool cleanup() = 0;
		virtual bool loadFromFile(const String & filename) = 0;
	};
}

#endif // !_I_RESOURCE_HPP_
