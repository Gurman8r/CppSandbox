#ifndef _VECTOR_4_HPP_
#define _VECTOR_4_HPP_

#include <MemeCore/Vector.hpp>

namespace ml
{
	template <typename T>
	class Vector4
		: public Vector<T, 4>
	{
	public:
		using Base = Vector<T, 4>;
		using Self = Vector4<T>;

	public:
		Vector4()
			: Base()
		{
		}

		Vector4(const T & xyzw)
			: Base(xyzw)
		{
		}

		Vector4(const T & x, const T & y, const T & z, const T & w)
			: Base({ x, y, z, w })
		{
		}

		template <size_t N>
		Vector4(const Vector<T, N> & copy)
			: Base(copy)
		{
		}

		template<typename U>
		Vector4(const Vector4<U> & copy)
			: Base(copy)
		{
		}

		template <typename U, size_t N>
		Vector4(const Vector<U, N> & copy)
			: Base(copy)
		{
		}

		virtual ~Vector4() {}

		static const Self Zero;
		static const Self One;
	};

	template<typename T> const Vector4<T> Vector4<T>::Zero = Vector4<T>(0);
	template<typename T> const Vector4<T> Vector4<T>::One  = Vector4<T>(1);

	using vec4f = Vector4<float>;
	using vec4i = Vector4<int32_t>;
	using vec4d = Vector4<double>;
	using vec4b = Vector4<uint8_t>;
	using vec4u = Vector4<uint32_t>;
}
#endif // !_VECTOR_4_HPP_
