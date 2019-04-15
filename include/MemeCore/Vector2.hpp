#ifndef _ML_VECTOR_2_HPP_
#define _ML_VECTOR_2_HPP_

#include <MemeCore/Vector.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> class Vector2
		: public Vector<T, 2>
	{
	public:
		using value_type	= typename T;
		using base_type		= typename Vector<value_type, 2>;
		using self_type		= typename Vector2<value_type>;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Vector2()
			: base_type()
		{
		}

		Vector2(const value_type xy)
			: base_type(xy)
		{
		}

		Vector2(const value_type x, const value_type y)
			: base_type({ x, y })
		{
		}
		
		template <
			size_t N
		> Vector2(const Vector<T, N> & copy)
			: base_type(copy)
		{
		}

		template <
			class U
		> Vector2(const Vector2<U>& copy)
			: base_type(copy)
		{
		}

		template <
			class U, 
			size_t N
		> Vector2(const Vector<U, N> & copy, const value_type dv = (value_type)(0))
			: base_type(copy, dv)
		{
		}

		virtual ~Vector2() {}
		
	public:
		/* * * * * * * * * * * * * * * * * * * * */
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

	public: // Constants
		/* * * * * * * * * * * * * * * * * * * * */

		static const self_type Zero;
		static const self_type One;
		static const self_type Up;
		static const self_type Down;
		static const self_type Left;
		static const self_type Right;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> const Vector2<T> Vector2<T>::Zero	= Vector2<T>( 0,  0);
	template <class T> const Vector2<T> Vector2<T>::One		= Vector2<T>( 1,  1);
	template <class T> const Vector2<T> Vector2<T>::Up		= Vector2<T>( 0,  1);
	template <class T> const Vector2<T> Vector2<T>::Down	= Vector2<T>( 0, -1);
	template <class T> const Vector2<T> Vector2<T>::Left	= Vector2<T>(-1,  0);
	template <class T> const Vector2<T> Vector2<T>::Right	= Vector2<T>( 1,  0);

	using vec2f = Vector2<float>;
	using vec2i = Vector2<int32_t>;
	using vec2d = Vector2<double>;
	using vec2b = Vector2<uint8_t>;
	using vec2u = Vector2<uint32_t>;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VECTOR_2_HPP_