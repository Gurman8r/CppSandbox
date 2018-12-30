#ifndef _VECTOR_3_H_
#define _VECTOR_3_H_

#include <MemeLib/Vector.h>

namespace ml
{
	template <typename T>
	class Vector3
		: public Vector<T, 3>
		, public IComparable<Vector<T, 3>>
	{
	public:
		using base_type = Vector<T, 3>;
		using self_type = Vector3<T>;

	public:
		Vector3()
			: Vector3((T)0, (T)0, (T)0)
		{
		}
		Vector3(const T & xyz)
			: Vector3(xyz, xyz, xyz)
		{
			(*this)[0] = xyz;
			(*this)[1] = xyz;
			(*this)[2] = xyz;
		}
		Vector3(const T & x, const T & y, const T & z)
		{
			(*this)[0] = x;
			(*this)[1] = y;
			(*this)[2] = z;
		}
		virtual ~Vector3() {}

	public:
		template <std::size_t N>
		Vector3(const Vector<T, N> & copy)
		{
			std::size_t num = std::min((*this).Size, copy.Size);
			for (std::size_t i = 0; i < num; i++)
			{
				(*this)[i] = copy[i];
			}
		}

		template<typename U>
		explicit Vector3(const Vector3<U>& copy)
			: Vector3(
				static_cast<T>(copy[0]), 
				static_cast<T>(copy[1]), 
				static_cast<T>(copy[2]))
		{
		}
		
		template <typename U, std::size_t N>
		explicit Vector3(const Vector<U, N> & copy)
		{
			std::size_t num = std::min((*this).Size, copy.Size);
			for (std::size_t i = 0; i < num; i++)
			{
				(*this)[i] = copy[i];
			}
		}


	public:
		inline virtual bool equals(const base_type & value) const override
		{
			for (std::size_t i = 0; i < (*this).Size; i++)
			{
				if ((*this)[i] != value[i])
				{
					return false;
				}
			}
			return true;
		}

		inline virtual bool lessThan(const base_type & value) const override
		{
			for (std::size_t i = 0; i < (*this).Size; i++)
			{
				if ((*this)[i] >= value[i])
				{
					return false;
				}
			}
			return true;
		}

	public:
		inline friend self_type		operator+(const self_type& lhs, const self_type& rhs)
		{
			return self_type(lhs[0] + rhs[0], lhs[1] + rhs[1]);
		};
		inline friend self_type		operator-(const self_type& lhs, const self_type& rhs)
		{
			return self_type(lhs[0] - rhs[0], lhs[1] - rhs[1]);
		};
		inline friend self_type		operator*(const self_type& lhs, const self_type& rhs)
		{
			return self_type(lhs[0] * rhs[0], lhs[1] * rhs[1]);
		};
		inline friend self_type		operator/(const self_type& lhs, const self_type& rhs)
		{
			return self_type(lhs[0] / rhs[0], lhs[1] / rhs[1]);
		};
		inline friend self_type		operator*(const self_type& lhs, const T rhs)
		{
			return self_type(lhs[0] * rhs, lhs[1] * rhs);
		};
		inline friend self_type		operator/(const self_type& lhs, const T rhs)
		{
			return self_type(lhs[0] / rhs, lhs[1] / rhs);
		};
		inline friend self_type &	operator+=(self_type& lhs, const self_type& rhs)
		{
			lhs = (lhs + rhs);
			return lhs;
		};
		inline friend self_type &	operator-=(self_type& lhs, const self_type& rhs)
		{
			lhs = (lhs - rhs);
			return lhs;
		};
		inline friend self_type &	operator*=(self_type& lhs, const self_type& rhs)
		{
			lhs = (lhs * rhs);
			return lhs;
		};
		inline friend self_type &	operator/=(self_type& lhs, const self_type& rhs)
		{
			lhs = (lhs / rhs);
			return lhs;
		};
		inline friend self_type &	operator*=(self_type& lhs, const T rhs)
		{
			lhs = (lhs * rhs);
			return lhs;
		};
		inline friend self_type &	operator/=(self_type& lhs, const T rhs)
		{
			lhs = (lhs / rhs);
			return lhs;
		};
		inline friend self_type		operator-(const self_type& rhs)
		{
			return rhs * static_cast<T>(-1);
		}
		
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
