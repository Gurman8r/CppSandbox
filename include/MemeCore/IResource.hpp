#ifndef _IRESOURCE_H_
#define _IRESOURCE_H_

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

#endif // !_IRESOURCE_H_
