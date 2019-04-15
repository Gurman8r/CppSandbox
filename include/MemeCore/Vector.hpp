#ifndef _ML_VECTOR_HPP_
#define _ML_VECTOR_HPP_

#include <MemeCore/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, 
		size_t N
	> class Vector
		: public Matrix<T, N, 1>
		, public IComparable<Vector<T, N>>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type	= typename T;
		using const_value	= typename const value_type;
		using self_type		= typename Vector<value_type, N>;
		using base_type		= typename Matrix<value_type, N, 1>;
		using initializer	= typename base_type::initializer;

	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Vector()
			: base_type()
		{
		}
		
		Vector(const_value value)
			: base_type(value)
		{
		}
		
		Vector(const_value * value)
			: base_type(value)
		{
		}
		
		Vector(const initializer & value)
			: base_type(value)
		{
		}
		
		Vector(const self_type & value)
			: base_type(value)
		{
		}

		template <
			size_t S
		> Vector(const Vector<T, S> & copy)
			: base_type(copy)
		{
		}

		template <
			class U
		> Vector(const Vector<U, N> & copy)
			: base_type(copy)
		{
		}

		template <
			class U, 
			size_t S
		> Vector(const Vector<U, S> & copy, const_value dv = (value_type)(0))
			: base_type(copy, dv)
		{
		}

		virtual ~Vector() {}

	public: // Overrides
		/* * * * * * * * * * * * * * * * * * * * */
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

	public: // Functions
		/* * * * * * * * * * * * * * * * * * * * */
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
		
		/* * * * * * * * * * * * * * * * * * * * */

		inline self_type & normalize()
		{
			return (*this) /= magnitude();
		};

		inline self_type normalized() const
		{
			return self_type(*this).normalize();
		};

		/* * * * * * * * * * * * * * * * * * * * */

		inline static float distance(const self_type & a, const self_type & b)
		{
			return self_type(a - b).magnitude();
		};

		inline static float dot(const self_type & a, const self_type & b)
		{
			float value = 0;
			for (size_t i = 0; i < self_type::Size; i++)
			{
				value += static_cast<float>(a[i]) * static_cast<float>(b[i]);
			}
			return value;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		inline static self_type direction(const self_type & from, const self_type & to)
		{
			return (to - from).normalized();
		};

		inline static self_type lerp(const self_type & a, const self_type & b, float t)
		{
			return Maths::lerp<self_type>(a, b, t);
		};

		inline static self_type reflect(const self_type & direction, const self_type & normal)
		{
			return (normal - direction) * ((T)2 * dot(direction, normal));
		};

	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * */
		template <
			class U
		> inline friend self_type operator+(const self_type & lhs, const Vector<U, N> & rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] + static_cast<T>(rhs[i]);
			}
			return tmp;
		};
		
		template <
			class U
		> inline friend self_type operator-(const self_type & lhs, const Vector<U, N> & rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] - static_cast<T>(rhs[i]);
			}
			return tmp;
		};
		
		template <
			class U
		> inline friend self_type operator*(const self_type & lhs, const Vector<U, N> & rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] * static_cast<T>(rhs[i]);
			}
			return tmp;
		};
		
		template <
			class U
		> inline friend self_type operator/(const self_type & lhs, const Vector<U, N> & rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] / static_cast<T>(rhs[i]);
			}
			return tmp;
		};
		
		inline friend self_type operator*(const self_type & lhs, const_value rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] * rhs;
			}
			return tmp;
		};
		
		inline friend self_type operator/(const self_type & lhs, const_value rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.Size; i++)
			{
				tmp[i] = lhs[i] / rhs;
			}
			return tmp;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class U
		> inline friend self_type & operator+=(self_type & lhs, const Vector<U, N> & rhs)
		{
			return (lhs = (lhs + rhs));
		};
		
		template <
			class U
		> inline friend self_type & operator-=(self_type & lhs, const Vector<U, N> & rhs)
		{
			return (lhs = (lhs - rhs));
		};
		
		template <
			class U
		> inline friend self_type & operator*=(self_type & lhs, const Vector<U, N> & rhs)
		{
			return (lhs = (lhs * rhs));
		};
		
		template <
			class U
		> inline friend self_type & operator/=(self_type & lhs, const Vector<U, N> & rhs)
		{
			return (lhs = (lhs / rhs));
		};
		
		inline friend self_type & operator*=(self_type & lhs, const_value rhs)
		{
			return (lhs = (lhs * rhs));
		};
		
		inline friend self_type & operator/=(self_type & lhs, const_value rhs)
		{
			return (lhs = (lhs / rhs));
		};
		
		/* * * * * * * * * * * * * * * * * * * * */

		inline friend self_type operator-(const self_type & rhs)
		{
			return rhs * static_cast<T>(-1);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VECTOR_HPP_