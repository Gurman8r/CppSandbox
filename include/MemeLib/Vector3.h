#ifndef _VECTOR_3_H_
#define _VECTOR_3_H_

#include <MemeLib/Vector.h>

namespace ml
{
	template <typename T>
	class Vector3
		: public Vector<T, 3>
	{
	public:
		using base_type = Vector<T, 3>;
		using self_type = Vector3<T>;

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

		template <std::size_t N>
		Vector3(const Vector<T, N> & copy)
			: base_type()
		{
			base_type::copyData(copy);
		}

		template<typename U>
		explicit Vector3(const Vector3<U>& copy)
			: base_type()
		{
			base_type::copyData(copy);
		}
		
		template <typename U, std::size_t N>
		explicit Vector3(const Vector<U, N> & copy)
			: base_type()
		{
			base_type::copyData(copy);
		}

		virtual ~Vector3() {}

	public:
		static const self_type Zero;
		static const self_type One;
		static const self_type Up;
		static const self_type Down;
		static const self_type Left;
		static const self_type Right;
		static const self_type Forward;
		static const self_type Backward;
	};

	template<typename T> const Vector3<T> Vector3<T>::Zero		= Vector3<T>( 0,  0,  0);
	template<typename T> const Vector3<T> Vector3<T>::One		= Vector3<T>( 1,  1,  1);
	template<typename T> const Vector3<T> Vector3<T>::Up		= Vector3<T>( 0,  1,  0);
	template<typename T> const Vector3<T> Vector3<T>::Down		= Vector3<T>( 0, -1,  0);
	template<typename T> const Vector3<T> Vector3<T>::Left		= Vector3<T>(-1,  0,  0);
	template<typename T> const Vector3<T> Vector3<T>::Right		= Vector3<T>( 1,  0,  0);
	template<typename T> const Vector3<T> Vector3<T>::Forward	= Vector3<T>( 0,  0,  1);
	template<typename T> const Vector3<T> Vector3<T>::Backward	= Vector3<T>( 0,  0, -1);

	using vec3f = Vector3<float>;
	using vec3i = Vector3<int>;
	using vec3d = Vector3<double>;
	using vec3u = Vector3<unsigned>;
}

#endif // !_VECTOR_3_H_
