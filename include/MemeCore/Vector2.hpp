#ifndef _VECTOR_2_HPP_
#define _VECTOR_2_HPP_

#include <MemeCore/Vector.hpp>

namespace ml
{
	template <typename T>
	class Vector2
		: public Vector<T, 2>
	{
	public:
		using Base = Vector<T, 2>;
		using Self = Vector2<T>;

	public:
		Vector2()
			: Base()
		{
		}

		Vector2(const T & xy)
			: Base(xy)
		{
		}

		Vector2(const T & x, const T & y)
			: Base({ x, y })
		{
		}
		
		template <size_t N>
		Vector2(const Vector<T, N> & copy)
			: Base(copy)
		{
		}

		template<typename U>
		Vector2(const Vector2<U>& copy)
			: Base(copy)
		{
		}

		template <typename U, size_t N>
		Vector2(const Vector<U, N> & copy)
			: Base(copy)
		{
		}

		virtual ~Vector2() {}
		
	public:
		inline static float angle(const Self & from, const Self & to)
		{
			return angle(to - from);
		};

		inline static float angle(const Self & value)
		{
			return atan2(value[0], value[1]);
		};

		inline static float det(const Self & a, const Self & b)
		{
			return 
				(static_cast<float>(a[0]) * static_cast<float>(b[1])) - 
				(static_cast<float>(a[1]) * static_cast<float>(b[0]));
		};

		inline static Self angle(float value)
		{
			return Self(cos(value), sin(value));
		};

	public:
		static const Self Zero;
		static const Self One;
		static const Self Up;
		static const Self Down;
		static const Self Left;
		static const Self Right;
	};

	template<typename T> const Vector2<T> Vector2<T>::Zero	= Vector2<T>( 0,  0);
	template<typename T> const Vector2<T> Vector2<T>::One	= Vector2<T>( 1,  1);
	template<typename T> const Vector2<T> Vector2<T>::Up	= Vector2<T>( 0,  1);
	template<typename T> const Vector2<T> Vector2<T>::Down	= Vector2<T>( 0, -1);
	template<typename T> const Vector2<T> Vector2<T>::Left	= Vector2<T>(-1,  0);
	template<typename T> const Vector2<T> Vector2<T>::Right = Vector2<T>( 1,  0);

	using vec2f = Vector2<float>;
	using vec2i = Vector2<int32_t>;
	using vec2d = Vector2<double>;
	using vec2b = Vector2<uint8_t>;
	using vec2u = Vector2<uint32_t>;
}

#endif // !_VECTOR_2_HPP_
