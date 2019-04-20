#ifndef _ML_VECTOR_2_HPP_
#define _ML_VECTOR_2_HPP_

#include <MemeCore/Vector.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Elem
	> class Vector2
		: public Vector<_Elem, 2>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type	= typename _Elem;
		using base_type		= typename Vector<value_type, 2>;
		using self_type		= typename Vector2<value_type>;

		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;


	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Vector2()
			: base_type()
		{
		}

		Vector2(const_reference xy)
			: base_type(xy)
		{
		}

		Vector2(const_reference x, const_reference y)
			: base_type({ x, y })
		{
		}
		
		template <
			size_t S
		> Vector2(const Vector<value_type, S> & copy)
			: base_type(copy)
		{
		}

		template <
			class U
		> Vector2(const Vector2<U> & copy)
			: base_type(copy)
		{
		}

		template <
			class U, 
			size_t S
		> Vector2(const Vector<U, S> & copy, const_reference def = static_cast<value_type>(0))
			: base_type(copy, def)
		{
		}

		virtual ~Vector2() {}

		
	public: // Static Functions
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
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Constants
	/* * * * * * * * * * * * * * * * * * * * */
	template <class E> const Vector2<E> Vector2<E>::Zero	= Vector2<E>( 0,  0);
	template <class E> const Vector2<E> Vector2<E>::One		= Vector2<E>( 1,  1);
	template <class E> const Vector2<E> Vector2<E>::Up		= Vector2<E>( 0,  1);
	template <class E> const Vector2<E> Vector2<E>::Down	= Vector2<E>( 0, -1);
	template <class E> const Vector2<E> Vector2<E>::Left	= Vector2<E>(-1,  0);
	template <class E> const Vector2<E> Vector2<E>::Right	= Vector2<E>( 1,  0);

	// Types
	/* * * * * * * * * * * * * * * * * * * * */
	using vec2f = Vector2<float>;
	using vec2i = Vector2<int32_t>;
	using vec2d = Vector2<double>;
	using vec2b = Vector2<uint8_t>;
	using vec2u = Vector2<uint32_t>;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_VECTOR_2_HPP_