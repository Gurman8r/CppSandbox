#ifndef _IRESOURCE_H_
#define _IRESOURCE_H_

#include <MemeCore/Export.h>
#include <string>

namespace ml
{
	class IResource
	{
	public:
		virtual bool cleanup() = 0;
		virtual bool loadFromFile(const std::string & filename) = 0;

	};
}

#endif // !_IRESOURCE_H_
