#ifndef _IHANDLE_H_
#define _IHANDLE_H_

#include <MemeGraphics/Export.h>

namespace ml
{
	class ML_GRAPHICS_API IHandle
	{
	public:
		using ID = uint32_t;

	public:
		IHandle(ID value) : m_id(value) {}
		virtual ~IHandle() {}

	public:
		inline const ID *	ptr()	const { return (&m_id); }
		inline const ID &	id()	const { return (m_id);	}
		inline bool			good()	const { return (m_id);	}

	public:
		inline operator const ID *()const { return ptr();	}
		inline operator const ID &()const { return id();	}
		inline operator bool()		const { return good();	}

	protected:
		inline ID & id() { return m_id; }

	private:
		ID m_id;
	};
}

#endif // !_IHANDLE_H_
