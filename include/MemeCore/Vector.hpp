#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include <MemeCore/Matrix.hpp>

namespace ml
{
	// One Dimensional Matrix
	template <class T, size_t N>
	class Vector
		: public Matrix<T, N, 1>
		, public IComparable<Vector<T, N>>
	{
	public:
		using base_type = Matrix<T, N, 1>;
		using self_type = Vector<T, N>;
		using data_type = T;

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

		template <size_t N>
		Vector(const Vector<T, N> & copy)
			: base_type(copy)
		{
		}

		template <class U>
		Vector(const Vector<U, N>& copy)
			: base_type(copy)
		{
		}

		template <class U, size_t N>
		Vector(const Vector<U, N> & copy, const data_type dv = (data_type)(0))
			: base_type(copy, dv)
		{
		}

		virtual ~Vector() {}

	public:
		inline virtual bool equals(const self_type & value) const override
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
		
		inline virtual bool lessThan(const self_type & value) const override
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
			for (size_t i = 0; i < (*this).Size; i++)
			{
				value += (*this)[i] * (*this)[i];
			}
			return value;
		};
		

		inline self_type & normalize()
		{
			return (*this) /= magnitude();
		};

		inline self_type normalized() const
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
			for (size_t i = 0; i < self_type::Size; i++)
			{
				value += static_cast<float>(a[i]) * static_cast<float>(b[i]);
			}
			return value;
		};


		inline static self_type direction(const self_type& from, const self_type& to)
		{
			return (to - from).normalized();
		};

		inline static self_type lerp(const self_type& a, const self_type& b, float t)
		{
			return Maths::lerp<self_type>(a, b, t);
		};

		inline static self_type reflect(const self_type& direction, const self_type& normal)
		{
			return (normal - direction) * ((T)2 * dot(direction, normal));
		};

	public:
		inline friend self_type operator+(const self_type& lhs, const self_type& rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] + rhs[i];
			}
			return tmp;
		};
		
		inline friend self_type operator-(const self_type& lhs, const self_type& rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] - rhs[i];
			}
			return tmp;
		};
		
		inline friend self_type operator*(const self_type& lhs, const self_type& rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] * rhs[i];
			}
			return tmp;
		};
		
		inline friend self_type operator/(const self_type& lhs, const self_type& rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] / rhs[i];
			}
			return tmp;
		};
		
		inline friend self_type operator*(const self_type& lhs, const T & rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] * rhs;
			}
			return tmp;
		};
		
		inline friend self_type operator/(const self_type& lhs, const T & rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.Size; i++)
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
		
		inline friend self_type & operator*=(self_type& lhs, const T & rhs)
		{
			return (lhs = (lhs * rhs));
		};
		
		inline friend self_type & operator/=(self_type& lhs, const T & rhs)
		{
			return (lhs = (lhs / rhs));
		};
		
		inline friend self_type operator-(const self_type& rhs)
		{
			return rhs * static_cast<T>(-1);
		}
	};
}

#endif // !_VECTOR_HPP_
