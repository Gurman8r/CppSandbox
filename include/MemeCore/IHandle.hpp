#ifndef _ML_I_HANDLE_HPP_
#define _ML_I_HANDLE_HPP_

#include <MemeCore/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T = uint32_t>
	class IHandle
	{
	public:
		using value_type		= T;
		using pointer			= typename value_type * ;
		using reference			= typename value_type & ;
		using const_pointer		= typename const value_type * ;
		using const_reference	= typename const value_type & ;

	public:
		IHandle(const value_type value)
			: m_ID(value)
		{
		}

		virtual ~IHandle() {}

	public:
		inline bool				good() const			{ return  m_ID; }
		inline reference		get_ref()				{ return  m_ID; }
		inline pointer			get_ptr()				{ return &m_ID; }
		inline const_pointer	get_const_ptr() const	{ return &m_ID; }
		inline const_reference	get_const_ref() const	{ return  m_ID; }

		inline void * get_address() { return (void *)(intptr_t)(get_ref()); }

	public:
		inline operator bool			() const	{ return good(); }
		inline operator reference		()			{ return get_ref(); }
		inline operator pointer			()			{ return get_ptr(); }
		inline operator const_pointer	() const	{ return get_const_ptr(); }
		inline operator const_reference	() const	{ return get_const_ref(); }
	
	private:
		mutable value_type m_ID;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_HANDLE_HPP_