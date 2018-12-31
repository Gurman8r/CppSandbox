#ifndef _VECTOR_2_H_
#define _VECTOR_2_H_

#include <MemeCore/Vector.h>

namespace ml
{
	template <typename T>
	class Vector2
		: public Vector<T, 2>
	{
	public:
		using base_type = Vector<T, 2>;
		using self_type = Vector2<T>;

	public:
		Vector2()
			: base_type()
		{
		}

		Vector2(const T & xy)
			: base_type(xy)
		{
		}

		Vector2(const T & x, const T & y)
			: base_type({ x, y })
		{
		}
		
		template <std::size_t N>
		Vector2(const Vector<T, N> & copy)
			: base_type(copy)
		{
		}

		template<typename U>
		Vector2(const Vector2<U>& copy)
			: base_type(copy)
		{
		}

		template <typename U, std::size_t N>
		Vector2(const Vector<U, N> & copy)
			: base_type(copy)
		{
		}

		virtual ~Vector2() {}
		
	public:
		inline static float angle(const self_type & from, const self_type & to)
		{
			return angle(to - from);
		};

		inline static float angle(const self_type & value)
		{
			return atan2(value[0], value[1]);
		};

		inline static float det(const self_type & a, const self_type & b)
		{
			return 
				(static_cast<float>(a[0]) * static_cast<float>(b[1])) - 
				(static_cast<float>(a[1]) * static_cast<float>(b[0]));
		};

		inline static self_type angle(float value)
		{
			return self_type(cos(value), sin(value));
		};

	public:
		static const self_type Zero;
		static const self_type One;
		static const self_type Up;
		static const self_type Down;
		static const self_type Left;
		static const self_type Right;
	};

	template<typename T> const Vector2<T> Vector2<T>::Zero	= Vector2<T>( 0,  0);
	template<typename T> const Vector2<T> Vector2<T>::One	= Vector2<T>( 1,  1);
	template<typename T> const Vector2<T> Vector2<T>::Up	= Vector2<T>( 0,  1);
	template<typename T> const Vector2<T> Vector2<T>::Down	= Vector2<T>( 0, -1);
	template<typename T> const Vector2<T> Vector2<T>::Left	= Vector2<T>(-1,  0);
	template<typename T> const Vector2<T> Vector2<T>::Right = Vector2<T>( 1,  0);

	using vec2f = Vector2<float>;
	using vec2i = Vector2<int>;
	using vec2d = Vector2<double>;
	using vec2u = Vector2<unsigned>;
}

#endif // !_VECTOR_2_H_
