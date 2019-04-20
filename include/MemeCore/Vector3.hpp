#ifndef _ML_VECTOR_3_HPP_
#define _ML_VECTOR_3_HPP_

#include <MemeCore/Vector.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Elem
	> class Vector3
		: public Vector<_Elem, 3>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type	= typename _Elem;
		using base_type		= typename Vector<value_type, 3>;
		using self_type		= typename Vector3<value_type>;

		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;


	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Vector3()
			: base_type()
		{
		}

		Vector3(const_reference xyz)
			: base_type(xyz)
		{
		}

		Vector3(const_reference x, const_reference y, const_reference z)
			: base_type({ x, y, z })
		{
		}

		template <
			size_t S
		> Vector3(const Vector<value_type, S> & copy)
			: base_type(copy)
		{
		}

		template <
			class U
		> Vector3(const Vector3<U>& copy)
			: base_type(copy)
		{
		}
		
		template <
			class U, 
			size_t S
		> Vector3(const Vector<U, S> & copy, const_reference def = static_cast<value_type>(0))
			: base_type(copy, def)
		{
		}

		virtual ~Vector3() {}


	public: // Member Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline self_type & cross(const self_type & other)
		{
			return (*this) = cross((*this), other);
		}

		inline self_type & project(const self_type & other)
		{
			return (*this) = project((*this), other);
		}


	public: // Static Functions
		/* * * * * * * * * * * * * * * * * * * * */
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


	public: // Constants
		/* * * * * * * * * * * * * * * * * * * * */
		static const self_type Zero;
		static const self_type One;
		static const self_type Up;
		static const self_type Down;
		static const self_type Left;
		static const self_type Right;
		static const self_type Forward;
		static const self_type Back;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Constants
	/* * * * * * * * * * * * * * * * * * * * */
	template <class E> const Vector3<E> Vector3<E>::Zero	= Vector3<E>( 0,  0,  0);
	template <class E> const Vector3<E> Vector3<E>::One		= Vector3<E>( 1,  1,  1);
	template <class E> const Vector3<E> Vector3<E>::Up		= Vector3<E>( 0,  1,  0);
	template <class E> const Vector3<E> Vector3<E>::Down	= Vector3<E>( 0, -1,  0);
	template <class E> const Vector3<E> Vector3<E>::Left	= Vector3<E>(-1,  0,  0);
	template <class E> const Vector3<E> Vector3<E>::Right	= Vector3<E>( 1,  0,  0);
	template <class E> const Vector3<E> Vector3<E>::Forward = Vector3<E>( 0,  0,  1);
	template <class E> const Vector3<E> Vector3<E>::Back	= Vector3<E>( 0,  0, -1);

	// Types
	/* * * * * * * * * * * * * * * * * * * * */
	using vec3f = Vector3<float>;
	using vec3i = Vector3<int32_t>;
	using vec3d = Vector3<double>;
	using vec3b = Vector3<uint8_t>;
	using vec3u = Vector3<uint32_t>;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_VECTOR_3_HPP_