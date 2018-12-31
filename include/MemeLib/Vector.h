#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <MemeLib/Matrix.h>

namespace ml
{
	// One Dimensional Matrix
	template <typename T, std::size_t N>
	class Vector
		: public Matrix<T, N, 1>
		, IComparable<Vector<T, N>>
	{
	public:
		using base_type = Matrix<T, N, 1>;
		using self_type = Vector<T, N>;

	public:
		Vector()
			: base_type()
		{
		}
		
		Vector(const T & value)
			: base_type(value)
		{
		}
		
		Vector(const T * value)
			: base_type(value)
		{
		}
		
		Vector(const std::initializer_list<T> & value)
			: base_type(value)
		{
		}
		
		Vector(const self_type & value)
			: base_type(value)
		{
		}

		template <std::size_t N>
		Vector(const Vector<T, N> & copy)
			: base_type(copy)
		{
		}

		template<typename U>
		Vector(const Vector<U, N>& copy)
			: base_type(copy)
		{
		}

		template <typename U, std::size_t N>
		Vector(const Vector<U, N> & copy)
			: base_type(copy)
		{
		}

		virtual ~Vector() {}

	public:
		inline virtual bool equals(const self_type & value) const override
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
		
		inline virtual bool lessThan(const self_type & value) const override
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
		inline float distanceTo(const self_type & value) const
		{
			return self_type::distance((*this), value);
		}

		inline float dot(const self_type & value) const
		{
			return self_type::dot((*this), value);
		}
		
		inline float magnitude() const
		{
			return sqrt(sqrMagnitude());
		};

		inline float sqrMagnitude() const
		{
			float value = 0;
			for (std::size_t i = 0; i < (*this).Size; i++)
			{
				value += (*this)[i] * (*this)[i];
			}
			return value;
		};
		

		inline self_type & normalize()
		{
			return (*this) /= magnitude();
		};

		inline self_type normal() const
		{
			return self_type(*this).normalize();
		};

		
		inline static float distance(const self_type& a, const self_type& b)
		{
			return self_type(a - b).magnitude();
		};

		inline static float dot(const self_type& a, const self_type& b)
		{
			float value = 0;
			for (std::size_t i = 0; i < self_type::Size; i++)
			{
				value += static_cast<float>(a[i]) * static_cast<float>(b[i]);
			}
			return value;
		};


		inline static self_type direction(const self_type& from, const self_type& to)
		{
			return (to - from).normal();
		};

		inline static self_type lerp(const self_type& a, const self_type& b, float t)
		{
			return self_type(a * t + b * ((T)(1) - t));
		};

		inline static self_type reflect(const self_type& direction, const self_type& normal)
		{
			return (normal - direction) * ((T)2 * dot(direction, normal));
		};

	public:
		inline friend self_type operator+(const self_type& lhs, const self_type& rhs)
		{
			self_type tmp;
			for (std::size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] + rhs[i];
			}
			return tmp;
		};
		
		inline friend self_type operator-(const self_type& lhs, const self_type& rhs)
		{
			self_type tmp;
			for (std::size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] + rhs[i];
			}
			return tmp;
		};
		
		inline friend self_type operator*(const self_type& lhs, const self_type& rhs)
		{
			self_type tmp;
			for (std::size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] * rhs[i];
			}
			return tmp;
		};
		
		inline friend self_type operator/(const self_type& lhs, const self_type& rhs)
		{
			self_type tmp;
			for (std::size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] / rhs[i];
			}
			return tmp;
		};
		
		inline friend self_type operator*(const self_type& lhs, float rhs)
		{
			self_type tmp;
			for (std::size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] * rhs;
			}
			return tmp;
		};
		
		inline friend self_type operator/(const self_type& lhs, float rhs)
		{
			self_type tmp;
			for (std::size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] / rhs;
			}
			return tmp;
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
		
		inline friend self_type & operator*=(self_type& lhs, const float rhs)
		{
			return (lhs = (lhs * rhs));
		};
		
		inline friend self_type & operator/=(self_type& lhs, const float rhs)
		{
			return (lhs = (lhs / rhs));
		};
		
		inline friend self_type operator-(const self_type& rhs)
		{
			return rhs * static_cast<T>(-1);
		}

	};
}

#endif // !_VECTOR_H_
