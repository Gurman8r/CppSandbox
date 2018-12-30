#ifndef _VECTOR_2_H_
#define _VECTOR_2_H_

#include <MemeLib/Vector.h>

namespace ml
{
	template <typename T>
	class Vector2
		: public Vector<T, 2>
	{
	public:
		using self_type = Vector2<T>;

	public:
		Vector2()
			: Vector2((T)0, (T)0)
		{
		}
		Vector2(const T & xy)
			: Vector2(xy, xy)
		{
		}
		Vector2(const T & xx, const T & yy)
		{
			(*this)[0] = xx;
			(*this)[1] = yy;
		}
		
		template <std::size_t N>
		Vector2(const Vector<T, N> & copy)
		{
			for (std::size_t i = 0, imax = std::min((*this).Size, copy.Size); i < imax; i++)
			{
				(*this)[i] = copy[i];
			}
		}

		template<typename U>
		explicit Vector2(const Vector2<U>& copy)
			: Vector2(
				static_cast<T>(copy[0]), 
				static_cast<T>(copy[1]))
		{
		}

		template <typename U, std::size_t N>
		explicit Vector2(const Vector<U, N> & copy)
		{
			for (std::size_t i = 0, imax = std::min((*this).Size, copy.Size); i < imax; i++)
			{
				(*this)[i] = copy[i];
			}
		}

		virtual ~Vector2() {}
		
	public:
		inline static float angle(const self_type& from, const self_type& to)
		{
			return angle(to - from);
		};

		inline static float angle(const self_type& dir)
		{
			return atan2(dir[0], dir[1]);
		};

		inline static float det(const self_type& a, const self_type& b)
		{
			return 
				(static_cast<float>(a[0]) * static_cast<float>(b[1])) - 
				(static_cast<float>(a[1]) * static_cast<float>(b[0]));
		};

		inline static self_type angle(float angle)
		{
			return self_type(self_type(cos(angle), sin(angle)));
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
