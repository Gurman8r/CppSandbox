#ifndef _VECTOR_4_H_
#define _VECTOR_4_H_

#include <MemeCore/Vector.h>

namespace ml
{
	template <typename T>
	class Vector4
		: public Vector<T, 4>
	{
	public:
		using base_type = Vector<T, 4>;
		using self_type = Vector4<T>;

	public:
		Vector4()
			: base_type()
		{
		}

		Vector4(const T & xyzw)
			: base_type(xyzw)
		{
		}

		Vector4(const T & x, const T & y, const T & z, const T & w)
			: base_type({ x, y, z, w })
		{
		}

		template <std::size_t N>
		Vector4(const Vector<T, N> & copy)
			: base_type(copy)
		{
		}

		template<typename U>
		Vector4(const Vector4<U>& copy)
			: base_type(copy)
		{
		}

		template <typename U, std::size_t N>
		Vector4(const Vector<U, N> & copy)
			: base_type(copy)
		{
		}

		virtual ~Vector4() {}

		static const self_type Zero;
		static const self_type One;
	};

	template<typename T> const Vector4<T> Vector4<T>::Zero = Vector2<T>(0);
	template<typename T> const Vector4<T> Vector4<T>::One = Vector2<T>(1);

	using vec4f = Vector4<float>;
	using vec4i = Vector4<int>;
	using vec4d = Vector4<double>;
	using vec4u = Vector4<unsigned>;
}
#endif // !_VECTOR_4_H_
