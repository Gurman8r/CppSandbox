#ifndef _ML_I_HANDLE_HPP_
#define _ML_I_HANDLE_HPP_

#include <MemeCore/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> class IHandle
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type		= typename T;
		using address			= typename void *;
		using pointer			= typename value_type * ;
		using reference			= typename value_type & ;
		using const_pointer		= typename const value_type * ;
		using const_reference	= typename const value_type & ;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		value_type m_handle;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		IHandle(const_reference value)
			: m_handle(value)
		{
		}

		virtual ~IHandle() {}

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline address get_address()
		{ 
			return (address)(intptr_t)(m_handle);
		}

		inline bool set_handle(const_reference value)
		{
			return ((!good()) && (get_reference() = (value)));
		}

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline bool				good() const			{ return  m_handle; }
		inline pointer			get_pointer()			{ return &m_handle; }
		inline reference		get_reference()			{ return  m_handle; }
		inline const_pointer	get_cpointer() const	{ return &m_handle; }
		inline const_reference	get_creference() const	{ return  m_handle; }

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline operator bool			() const		{ return good(); }
		inline operator pointer			()				{ return get_pointer(); }
		inline operator reference		()				{ return get_reference(); }
		inline operator const_pointer	() const		{ return get_cpointer(); }
		inline operator const_reference	() const		{ return get_creference(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_HANDLE_HPP_