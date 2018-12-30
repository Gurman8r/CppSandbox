#ifndef _VECTOR_2_H_
#define _VECTOR_2_H_

#include <MemeLib/Vector.h>

namespace ml
{
	template <typename T>
	class Vector2
		: public Vector<T, 2>
		, public IComparable< Vector<T, 2>>
	{
	public:
		using base_type = Vector<T, 2>;
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
		virtual ~Vector2() {}

	public:
		template <std::size_t N>
		Vector2(const Vector<T, N> & copy)
		{
			std::size_t num = std::min((*this).Size, copy.Size);
			for (std::size_t i = 0; i < num; i++)
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
			std::size_t num = std::min((*this).Size, copy.Size);
			for (std::size_t i = 0; i < num; i++)
			{
				(*this)[i] = copy[i];
			}
		}


	public:
		inline bool equals(const base_type & value) const override
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
		
		inline bool lessThan(const base_type & value) const override
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
		inline T magnitude() const
		{
			return sqrt(sqrMagnitude());
		};
		inline T sqrMagnitude() const
		{
			T value = 0;
			for (unsigned i = 0; i < (*this).Size; i++)
			{
				value += (*this)[i] * (*this)[i];
			}
			return value;
		};
		inline self_type & normalize()
		{
			return ((*this) = ((T)1 / sqrt(sqrMagnitude())));
		};
		inline self_type normal() const
		{
			return self_type(*this).normalize();
		};

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
		inline static float distance(const self_type& a, const self_type& b)
		{
			return Vector2<float>(a - b).magnitude();
		};
		inline static float dot(const self_type& a, const self_type& b)
		{
			float value = 0;
			for (std::size_t i = 0; i < (*this).Size; i++)
			{
				value += static_cast<float>(a[i]) * static_cast<float>(b[i]);
			}
			return value;
		};

		inline static self_type angle(float angle)
		{
			return self_type(Vector2<float>(cos(angle), sin(angle)));
		};
		inline static self_type direction(const self_type& from, const self_type& to)
		{
			return self_type(Vector2<float>(to - from)).normal();
		};
		inline static self_type lengthDir(float length, float dir)
		{
			return self_type(Vector2<float>::angle(dir) * length);
		};
		inline static self_type lerp(const self_type& a, const self_type& b, float t)
		{
			return self_type(Vector2<float>(a * t + b * (static_cast<T>(1) - t)));
		};
		inline static self_type reflect(const self_type& direction, const self_type& normal)
		{
			return (normal - direction) * (static_cast<T>(2) * dot(direction, normal));
		};

	public:
		inline friend self_type operator+(const self_type& lhs, const self_type& rhs)
		{
			return self_type(lhs[0] + rhs[0], lhs[1] + rhs[1]);
		};
		inline friend self_type operator-(const self_type& lhs, const self_type& rhs)
		{
			return self_type(lhs[0] - rhs[0], lhs[1] - rhs[1]);
		};
		inline friend self_type operator*(const self_type& lhs, const self_type& rhs)
		{
			return self_type(lhs[0] * rhs[0], lhs[1] * rhs[1]);
		};
		inline friend self_type operator/(const self_type& lhs, const self_type& rhs)
		{
			return self_type(lhs[0] / rhs[0], lhs[1] / rhs[1]);
		};
		inline friend self_type operator*(const self_type& lhs, const T& rhs)
		{
			return self_type(lhs[0] * rhs, lhs[1] * rhs);
		};
		inline friend self_type operator/(const self_type& lhs, const T& rhs)
		{
			return self_type(lhs[0] / rhs, lhs[1] / rhs);
		};
		inline friend self_type & operator+=(self_type& lhs, const self_type& rhs)
		{
			return (lhs = (lhs + rhs));
		};
		inline friend self_type & operator-=(self_type& lhs, const self_type& rhs)
		{
			return (lhs = (lhs - rhs));
		};
		inline friend self_type & operator*=(self_type& lhs, const self_type& rhs)
		{
			return (lhs = (lhs * rhs));
		};
		inline friend self_type & operator/=(self_type& lhs, const self_type& rhs)
		{
			return (lhs = (lhs / rhs));
		};
		inline friend self_type & operator*=(self_type& lhs, const T& rhs)
		{
			return (lhs = (lhs * rhs));
		};
		inline friend self_type & operator/=(self_type& lhs, const T& rhs)
		{
			return (lhs = (lhs / rhs));
		};
		inline friend self_type operator-(const self_type& rhs)
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
