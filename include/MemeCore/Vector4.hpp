#ifndef _VECTOR_4_HPP_
#define _VECTOR_4_HPP_

#include <MemeCore/Vector.hpp>

namespace ml
{
	template <class _Elem>
	class Vector4
		: public Vector<_Elem, 4>
	{
	public:
		using base_type		= Vector<_Elem, 4>;
		using self_type		= Vector4<_Elem>;
		using value_type	= _Elem;

	public:
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

		template <size_t N>
		Vector4(const Vector<_Elem, N> & copy)
			: base_type(copy)
		{
		}

		template <class U>
		Vector4(const Vector4<U> & copy)
			: base_type(copy)
		{
		}

		template <class U, size_t N>
		Vector4(const Vector<U, N> & copy, const value_type dv = (value_type)(0))
			: base_type(copy, dv)
		{
		}

		virtual ~Vector4() {}

	public:
		static const self_type Zero;
		static const self_type One;
	};

	template <class _Elem> const Vector4<_Elem> Vector4<_Elem>::Zero = Vector4<_Elem>(0);
	template <class _Elem> const Vector4<_Elem> Vector4<_Elem>::One  = Vector4<_Elem>(1);

	using vec4f = Vector4<float>;
	using vec4i = Vector4<int32_t>;
	using vec4d = Vector4<double>;
	using vec4b = Vector4<uint8_t>;
	using vec4u = Vector4<uint32_t>;
}
#endif // !_VECTOR_4_HPP_
