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
		IHandle() 
			: IHandle(NULL) 
		{}
		IHandle(ID id)
			: m_id(id) 
		{}
		virtual ~IHandle() {}

	public:
		inline const ID *	ptr()	const { return (&m_id); }
		inline const ID &	id()	const { return (m_id); }
		inline bool			good()	const { return (m_id != NULL); }

	public:
		inline operator const ID *() const { return ptr(); }
		inline operator const ID &() const { return id(); }
		inline operator bool()		 const { return good(); }

	protected:
		ID m_id;
	};
}

#endif // !_IHANDLE_H_
