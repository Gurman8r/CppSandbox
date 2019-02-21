#ifndef _VECTOR_2_HPP_
#define _VECTOR_2_HPP_

#include <MemeCore/Vector.hpp>

namespace ml
{
	template <class _Elem>
	class Vector2
		: public Vector<_Elem, 2>
	{
	public:
		using base_type = Vector<_Elem, 2>;
		using self_type = Vector2<_Elem>;
		using value_type = _Elem;

	public:
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
		
		template <size_t N>
		Vector2(const Vector<_Elem, N> & copy)
			: base_type(copy)
		{
		}

		template <class U>
		Vector2(const Vector2<U>& copy)
			: base_type(copy)
		{
		}

		template <class U, size_t N>
		Vector2(const Vector<U, N> & copy, const value_type dv = (value_type)(0))
			: base_type(copy, dv)
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

	template <class _Elem> const Vector2<_Elem> Vector2<_Elem>::Zero	= Vector2<_Elem>( 0,  0);
	template <class _Elem> const Vector2<_Elem> Vector2<_Elem>::One		= Vector2<_Elem>( 1,  1);
	template <class _Elem> const Vector2<_Elem> Vector2<_Elem>::Up		= Vector2<_Elem>( 0,  1);
	template <class _Elem> const Vector2<_Elem> Vector2<_Elem>::Down	= Vector2<_Elem>( 0, -1);
	template <class _Elem> const Vector2<_Elem> Vector2<_Elem>::Left	= Vector2<_Elem>(-1,  0);
	template <class _Elem> const Vector2<_Elem> Vector2<_Elem>::Right	= Vector2<_Elem>( 1,  0);

	using vec2f = Vector2<float>;
	using vec2i = Vector2<int32_t>;
	using vec2d = Vector2<double>;
	using vec2b = Vector2<uint8_t>;
	using vec2u = Vector2<uint32_t>;
}

#endif // !_VECTOR_2_HPP_
