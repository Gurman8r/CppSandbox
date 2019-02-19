#ifndef _VECTOR_3_HPP_
#define _VECTOR_3_HPP_

#include <MemeCore/Vector.hpp>

namespace ml
{
	template <class T>
	class Vector3
		: public Vector<T, 3>
	{
	public:
		using base_type = Vector<T, 3>;
		using self_type = Vector3<T>;
		using data_type = T;

	public:
		Vector3()
			: base_type()
		{
		}

		Vector3(const T & xyz)
			: base_type(xyz)
		{
		}

		Vector3(const T & x, const T & y, const T & z)
			: base_type({ x, y, z })
		{
		}

		template <size_t N>
		Vector3(const Vector<T, N> & copy)
			: base_type(copy)
		{
		}

		template <class U>
		Vector3(const Vector3<U>& copy)
			: base_type(copy)
		{
		}
		
		template <class U, size_t N>
		Vector3(const Vector<U, N> & copy, const data_type dv = (data_type)(0))
			: base_type(copy, dv)
		{
		}

		virtual ~Vector3() {}

	public:
		inline self_type & cross(const self_type & other)
		{
			return (*this) = cross((*this), other);
		}

		inline self_type & project(const self_type & other)
		{
			return (*this) = project((*this), other);
		}

		inline static self_type cross(const self_type & a, const self_type & b)
		{
			// (y - z) - (z - y)
			// (z - x) - (x - z)
			// (x - y) - (y - x)
			return self_type(
				(a[1] * b[2]) - (a[2] * b[1]),
				(a[2] * b[0]) - (a[0] * b[2]),
				(a[0] * b[1]) - (a[1] * b[0]));
		}

		inline static self_type project(const self_type & a, const self_type & b)
		{
			return self_type();
		}

	public:
		static const self_type Zero;
		static const self_type One;
		static const self_type Up;
		static const self_type Down;
		static const self_type Left;
		static const self_type Right;
		static const self_type Forward;
		static const self_type Back;
	};

	template <class T> const Vector3<T> Vector3<T>::Zero	= Vector3<T>( 0,  0,  0);
	template <class T> const Vector3<T> Vector3<T>::One		= Vector3<T>( 1,  1,  1);
	template <class T> const Vector3<T> Vector3<T>::Up		= Vector3<T>( 0,  1,  0);
	template <class T> const Vector3<T> Vector3<T>::Down	= Vector3<T>( 0, -1,  0);
	template <class T> const Vector3<T> Vector3<T>::Left	= Vector3<T>(-1,  0,  0);
	template <class T> const Vector3<T> Vector3<T>::Right	= Vector3<T>( 1,  0,  0);
	template <class T> const Vector3<T> Vector3<T>::Forward	= Vector3<T>( 0,  0,  1);
	template <class T> const Vector3<T> Vector3<T>::Back	= Vector3<T>( 0,  0, -1);

	using vec3f = Vector3<float>;
	using vec3i = Vector3<int32_t>;
	using vec3d = Vector3<double>;
	using vec3b = Vector3<uint8_t>;
	using vec3u = Vector3<uint32_t>;
}

#endif // !_VECTOR_3_HPP_
