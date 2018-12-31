#ifndef _INON_COPYABLE_H_
#define _INON_COPYABLE_H_

#include <MemeLib/Export.h>

namespace ml
{
	class ML_CORE_API INonCopyable
	{
	protected:
		INonCopyable() {}
		~INonCopyable() {}

	private:
		INonCopyable(const INonCopyable&) = delete;
		INonCopyable & operator=(const INonCopyable&) = delete;
	};
}

#endif // !_INON_COPYABLE_H_
