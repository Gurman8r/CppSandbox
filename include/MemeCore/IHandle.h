#ifndef _IHANDLE_H_
#define _IHANDLE_H_

#include <inttypes.h>

namespace ml
{
	template <typename ID = uint32_t>
	class IHandle
	{
	private:
		ID m_ID;

	public:
		IHandle(ID value)
			: m_ID(value)
		{
		}
		virtual ~IHandle() {}

	public:
		inline ID & handle() { return m_ID; }

	public:
		inline const ID *	const_ptr()	const { return &m_ID; }
		inline const ID &	const_ref()	const { return  m_ID; }
		inline bool			good()		const { return  m_ID; }

	public:
		inline operator const ID *()	const { return const_ptr();	}
		inline operator const ID &()	const { return const_ref();	}
		inline operator bool()			const { return good();		}

	};
}

#endif // !_IHANDLE_H_
