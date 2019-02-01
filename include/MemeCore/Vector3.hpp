#ifndef _VECTOR_3_HPP_
#define _VECTOR_3_HPP_

#include <MemeCore/Vector.hpp>

namespace ml
{
	template <typename T>
	class Vector3
		: public Vector<T, 3>
	{
	public:
		using Base = Vector<T, 3>;
		using Self = Vector3<T>;

	public:
		Vector3()
			: Base()
		{
		}

		Vector3(const T & xyz)
			: Base(xyz)
		{
		}

		Vector3(const T & x, const T & y, const T & z)
			: Base({ x, y, z })
		{
		}

		template <size_t N>
		Vector3(const Vector<T, N> & copy)
			: Base(copy)
		{
		}

		template<typename U>
		Vector3(const Vector3<U>& copy)
			: Base(copy)
		{
		}
		
		template <typename U, size_t N>
		Vector3(const Vector<U, N> & copy)
			: Base(copy)
		{
		}

		virtual ~Vector3() {}

	public:
		static const Self Zero;
		static const Self One;
		static const Self Up;
		static const Self Down;
		static const Self Left;
		static const Self Right;
		static const Self Forward;
		static const Self Back;
	};

	template<typename T> const Vector3<T> Vector3<T>::Zero		= Vector3<T>( 0,  0,  0);
	template<typename T> const Vector3<T> Vector3<T>::One		= Vector3<T>( 1,  1,  1);
	template<typename T> const Vector3<T> Vector3<T>::Up		= Vector3<T>( 0,  1,  0);
	template<typename T> const Vector3<T> Vector3<T>::Down		= Vector3<T>( 0, -1,  0);
	template<typename T> const Vector3<T> Vector3<T>::Left		= Vector3<T>(-1,  0,  0);
	template<typename T> const Vector3<T> Vector3<T>::Right		= Vector3<T>( 1,  0,  0);
	template<typename T> const Vector3<T> Vector3<T>::Forward	= Vector3<T>( 0,  0,  1);
	template<typename T> const Vector3<T> Vector3<T>::Back		= Vector3<T>( 0,  0, -1);

	using vec3f = Vector3<float>;
	using vec3i = Vector3<int32_t>;
	using vec3d = Vector3<double>;
	using vec3b = Vector3<uint8_t>;
	using vec3u = Vector3<uint32_t>;
}

#endif // !_VECTOR_3_HPP_
