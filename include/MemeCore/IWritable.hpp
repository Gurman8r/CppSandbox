#ifndef _I_WRITABLE_HPP_
#define _I_WRITABLE_HPP_

#include <MemeCore/String.hpp>

namespace ml
{
	class IWritable
	{
	public:
		virtual bool writeFile(const String & filename) const = 0;
	};
}

#endif // !_I_WRITABLE_HPP_