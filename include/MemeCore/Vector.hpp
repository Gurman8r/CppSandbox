#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include <MemeCore/Matrix.hpp>

namespace ml
{
	// One Dimensional Matrix
	template <typename T, size_t N>
	class Vector
		: public Matrix<T, N, 1>
		, public IComparable<Vector<T, N>>
	{
	public:
		using Base = Matrix<T, N, 1>;
		using Self = Vector<T, N>;

	public:
		Vector()
			: Base()
		{
		}
		
		Vector(const T & value)
			: Base(value)
		{
		}
		
		Vector(const T * value)
			: Base(value)
		{
		}
		
		Vector(const std::initializer_list<T> & value)
			: Base(value)
		{
		}
		
		Vector(const Self & value)
			: Base(value)
		{
		}

		template <size_t N>
		Vector(const Vector<T, N> & copy)
			: Base(copy)
		{
		}

		template<typename U>
		Vector(const Vector<U, N>& copy)
			: Base(copy)
		{
		}

		template <typename U, size_t N>
		Vector(const Vector<U, N> & copy)
			: Base(copy)
		{
		}

		virtual ~Vector() {}

	public:
		inline virtual bool equals(const Self & value) const override
		{
			for (size_t i = 0; i < (*this).Size; i++)
			{
				if ((*this)[i] != value[i])
				{
					return false;
				}
			}
			return true;
		}
		
		inline virtual bool lessThan(const Self & value) const override
		{
			for (size_t i = 0; i < (*this).Size; i++)
			{
				if ((*this)[i] >= value[i])
				{
					return false;
				}
			}
			return true;
		}


	public:
		inline float distanceTo(const Self & value) const
		{
			return Self::distance((*this), value);
		}

		inline float dot(const Self & value) const
		{
			return Self::dot((*this), value);
		}
		
		inline float magnitude() const
		{
			return sqrt(sqrMagnitude());
		};

		inline float sqrMagnitude() const
		{
			float value = 0;
			for (size_t i = 0; i < (*this).Size; i++)
			{
				value += (*this)[i] * (*this)[i];
			}
			return value;
		};
		

		inline Self & normalize()
		{
			return (*this) /= magnitude();
		};

		inline Self normal() const
		{
			return Self(*this).normalize();
		};

		
		inline static float distance(const Self& a, const Self& b)
		{
			return Self(a - b).magnitude();
		};

		inline static float dot(const Self& a, const Self& b)
		{
			float value = 0;
			for (size_t i = 0; i < Self::Size; i++)
			{
				value += static_cast<float>(a[i]) * static_cast<float>(b[i]);
			}
			return value;
		};


		inline static Self direction(const Self& from, const Self& to)
		{
			return (to - from).normal();
		};

		inline static Self lerp(const Self& a, const Self& b, float t)
		{
			return Self(a * t + b * ((T)(1) - t));
		};

		inline static Self reflect(const Self& direction, const Self& normal)
		{
			return (normal - direction) * ((T)2 * dot(direction, normal));
		};

	public:
		inline friend Self operator+(const Self& lhs, const Self& rhs)
		{
			Self tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] + rhs[i];
			}
			return tmp;
		};
		
		inline friend Self operator-(const Self& lhs, const Self& rhs)
		{
			Self tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] - rhs[i];
			}
			return tmp;
		};
		
		inline friend Self operator*(const Self& lhs, const Self& rhs)
		{
			Self tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] * rhs[i];
			}
			return tmp;
		};
		
		inline friend Self operator/(const Self& lhs, const Self& rhs)
		{
			Self tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] / rhs[i];
			}
			return tmp;
		};
		
		inline friend Self operator*(const Self& lhs, const T & rhs)
		{
			Self tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] * rhs;
			}
			return tmp;
		};
		
		inline friend Self operator/(const Self& lhs, const T & rhs)
		{
			Self tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] / rhs;
			}
			return tmp;
		};
		

		inline friend Self & operator+=(Self& lhs, const Self& rhs)
		{
			return (lhs = (lhs + rhs));
		};
		
		inline friend Self & operator-=(Self& lhs, const Self& rhs)
		{
			return (lhs = (lhs - rhs));
		};
		
		inline friend Self & operator*=(Self& lhs, const Self& rhs)
		{
			return (lhs = (lhs * rhs));
		};
		
		inline friend Self & operator/=(Self& lhs, const Self& rhs)
		{
			return (lhs = (lhs / rhs));
		};
		
		inline friend Self & operator*=(Self& lhs, const T & rhs)
		{
			return (lhs = (lhs * rhs));
		};
		
		inline friend Self & operator/=(Self& lhs, const T & rhs)
		{
			return (lhs = (lhs / rhs));
		};
		
		inline friend Self operator-(const Self& rhs)
		{
			return rhs * static_cast<T>(-1);
		}
	};
}

#endif // !_VECTOR_HPP_
