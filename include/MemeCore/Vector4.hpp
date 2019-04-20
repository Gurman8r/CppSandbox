#ifndef _ML_VECTOR_4_HPP_
#define _ML_VECTOR_4_HPP_

#include <MemeCore/Vector.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Elem
	> class Vector4
		: public Vector<_Elem, 4>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type	= typename _Elem;
		using base_type		= Vector<value_type, 4>;
		using self_type		= Vector4<value_type>;

		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;


	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Vector4()
			: base_type()
		{
		}

		Vector4(const_reference xyzw)
			: base_type(xyzw)
		{
		}

		Vector4(const_reference x, const_reference y, const_reference z, const_reference w)
			: base_type({ x, y, z, w })
		{
		}

		template <
			size_t S
		> Vector4(const Vector<value_type, S> & copy)
			: base_type(copy)
		{
		}

		template <
			class U
		> Vector4(const Vector4<U> & copy)
			: base_type(copy)
		{
		}

		template <
			class U, 
			size_t S
		> Vector4(const Vector<U, S> & copy, const_reference def = static_cast<value_type>(0))
			: base_type(copy, def)
		{
		}

		virtual ~Vector4() {}


	public: // Constants
		/* * * * * * * * * * * * * * * * * * * * */
		static const self_type Zero;
		static const self_type One;
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Constants
	/* * * * * * * * * * * * * * * * * * * * */
	template <class E> const Vector4<E> Vector4<E>::Zero	= Vector4<E>(0);
	template <class E> const Vector4<E> Vector4<E>::One		= Vector4<E>(1);

	// Types
	/* * * * * * * * * * * * * * * * * * * * */
	using vec4f = Vector4<float>;
	using vec4i = Vector4<int32_t>;
	using vec4d = Vector4<double>;
	using vec4b = Vector4<uint8_t>;
	using vec4u = Vector4<uint32_t>;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_VECTOR_4_HPP_