#ifndef _ML_I_HANDLE_HPP_
#define _ML_I_HANDLE_HPP_

#include <MemeCore/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> class IHandle
	{
	public:
		using value_type		= typename T;
		using pointer			= typename value_type * ;
		using reference			= typename value_type & ;
		using const_pointer		= typename const value_type * ;
		using const_reference	= typename const value_type & ;

	public:
		IHandle(const value_type value)
			: m_handle(value)
		{
		}

		virtual ~IHandle() {}

	public:
		inline void * get_address() { return (void *)(intptr_t)(get_ref()); }

	public:
		inline bool				good() const			{ return  m_handle; }
		inline reference		get_ref()				{ return  m_handle; }
		inline pointer			get_ptr()				{ return &m_handle; }
		inline const_pointer	get_const_ptr() const	{ return &m_handle; }
		inline const_reference	get_const_ref() const	{ return  m_handle; }

	public:
		inline operator bool			() const		{ return good(); }
		inline operator reference		()				{ return get_ref(); }
		inline operator pointer			()				{ return get_ptr(); }
		inline operator const_pointer	() const		{ return get_const_ptr(); }
		inline operator const_reference	() const		{ return get_const_ref(); }
	
	private:
		value_type m_handle;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_HANDLE_HPP_