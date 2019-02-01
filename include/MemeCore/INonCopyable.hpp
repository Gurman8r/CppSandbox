#ifndef _INON_COPYABLE_HPP_
#define _INON_COPYABLE_HPP_

#include <MemeCore/Export.hpp>

namespace ml
{
	class ML_CORE_API INonCopyable
	{
	protected:
		INonCopyable() {}
		~INonCopyable() {}

	private:
		INonCopyable(const INonCopyable &) = delete;
		INonCopyable & operator=(const INonCopyable &) = delete;
	};
}

#endif // !_INON_COPYABLE_HPP_
