#ifndef _ML_VECTOR_4_HPP_
#define _ML_VECTOR_4_HPP_

#include <MemeCore/Vector.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> class Vector4
		: public Vector<T, 4>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type	= typename T;
		using const_value	= typename const value_type;
		using base_type		= Vector<value_type, 4>;
		using self_type		= Vector4<value_type>;

	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Vector4()
			: base_type()
		{
		}

		Vector4(const_value xyzw)
			: base_type(xyzw)
		{
		}

		Vector4(const_value x, const_value y, const_value z, const_value w)
			: base_type({ x, y, z, w })
		{
		}

		template <
			size_t S
		> Vector4(const Vector<T, S> & copy)
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
		> Vector4(const Vector<U, S> & copy, const_value dv = (value_type)(0))
			: base_type(copy, dv)
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
	template <class T> const Vector4<T> Vector4<T>::Zero = Vector4<T>(0);
	template <class T> const Vector4<T> Vector4<T>::One = Vector4<T>(1);

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