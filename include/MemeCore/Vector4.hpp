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
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type	= typename T;
		using base_type		= Vector<value_type, 4>;
		using self_type		= Vector4<value_type>;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Vector4()
			: base_type()
		{
		}

		Vector4(const value_type xyzw)
			: base_type(xyzw)
		{
		}

		Vector4(const value_type x, const value_type y, const value_type z, const value_type w)
			: base_type({ x, y, z, w })
		{
		}

		template <
			size_t N
		> Vector4(const Vector<T, N> & copy)
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
			size_t N
		> Vector4(const Vector<U, N> & copy, const value_type dv = (value_type)(0))
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

	// Usings
	/* * * * * * * * * * * * * * * * * * * * */
	using vec4f = Vector4<float>;
	using vec4i = Vector4<int32_t>;
	using vec4d = Vector4<double>;
	using vec4b = Vector4<uint8_t>;
	using vec4u = Vector4<uint32_t>;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_VECTOR_4_HPP_