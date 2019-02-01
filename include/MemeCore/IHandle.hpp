#ifndef _IHANDLE_HPP_
#define _IHANDLE_HPP_

#include <MemeCore/IntTypes.hpp>

namespace ml
{
	template <typename T = uint32_t>
	class IHandle
	{
	public:
		using id_type = T;
	
	public:
		IHandle(id_type value)
			: m_ID(value)
		{
		}
		virtual ~IHandle() {}

	public:
		inline bool	good() const { return m_ID; }

		inline id_type & get_ref() { return m_ID; }
		
		inline id_type * get_ptr() { return &m_ID; }
	 							  
		inline const id_type * get_const_ptr() const { return &m_ID; }
		
		inline const id_type & get_const_ref() const { return  m_ID; }

	public:
		inline operator bool() const { return good(); }
		
		inline operator id_type &() { return get_ref(); }
		
		inline operator id_type *() { return get_ptr(); }

		inline operator const id_type *() const { return get_const_ptr(); }
		
		inline operator const id_type &() const { return get_const_ref(); }
	
	private:
		id_type m_ID;
	};
}

#endif // !_IHANDLE_HPP_
