#ifndef _ML_VECTOR_HPP_
#define _ML_VECTOR_HPP_

#include <MemeCore/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class	_Elem, 
		size_t	_Size
	> class Vector
		: public Matrix<_Elem, _Size, 1>
		, public IComparable<Vector<_Elem, _Size>>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type		= typename _Elem;
		using self_type			= typename Vector<value_type, _Size>;
		using base_type			= typename Matrix<value_type, _Size, 1>;
		using init_type			= typename base_type::init_type;

		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;


	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Vector()
			: base_type()
		{
		}
		
		Vector(const_reference value)
			: base_type(value)
		{
		}
		
		Vector(const_pointer value)
			: base_type(value)
		{
		}
		
		Vector(const init_type & value)
			: base_type(value)
		{
		}
		
		Vector(const self_type & value)
			: base_type(value)
		{
		}

		template <
			size_t S
		> Vector(const Vector<value_type, S> & copy)
			: base_type(copy)
		{
		}

		template <
			class E
		> Vector(const Vector<E, base_type::Size> & copy)
			: base_type(copy)
		{
		}

		template <
			class E, 
			size_t S
		> Vector(const Vector<E, S> & copy, const_reference def = static_cast<value_type>(0))
			: base_type(copy, def)
		{
		}

		virtual ~Vector() {}


	public: // Overrides
		/* * * * * * * * * * * * * * * * * * * * */
		inline virtual bool equals(const self_type & value) const override
		{
			for (size_t i = 0; i < (*this).size(); i++)
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
			for (size_t i = 0; i < (*this).size(); i++)
			{
				if ((*this)[i] >= value[i])
				{
					return false;
				}
			}
			return true;
		}


	public: // Member Functions
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
			for (size_t i = 0; i < (*this).size(); i++)
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


	public: // Static Functions
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
			return (normal - direction) * (static_cast<value_type>(2) * dot(direction, normal));
		};


	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * */
		inline friend self_type operator-(const self_type & rhs)
		{
			return (rhs * static_cast<value_type>(-1));
		}

		inline friend self_type operator*(const self_type & lhs, const_reference rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.size(); i++)
			{
				tmp[i] = lhs[i] * rhs;
			}
			return tmp;
		};

		inline friend self_type operator/(const self_type & lhs, const_reference rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.size(); i++)
			{
				tmp[i] = lhs[i] / rhs;
			}
			return tmp;
		};

		inline friend self_type & operator*=(self_type & lhs, const_reference rhs)
		{
			return (lhs = (lhs * rhs));
		};

		inline friend self_type & operator/=(self_type & lhs, const_reference rhs)
		{
			return (lhs = (lhs / rhs));
		};

		/* * * * * * * * * * * * * * * * * * * * */

		template <class E> 
		inline friend self_type operator+(const self_type & lhs, const Vector<E, base_type::Size> & rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.size(); i++)
			{
				tmp[i] = lhs[i] + static_cast<value_type>(rhs[i]);
			}
			return tmp;
		};
		
		template <class E>
		inline friend self_type operator-(const self_type & lhs, const Vector<E, base_type::Size> & rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.size(); i++)
			{
				tmp[i] = lhs[i] - static_cast<value_type>(rhs[i]);
			}
			return tmp;
		};
		
		template <class E>
		inline friend self_type operator*(const self_type & lhs, const Vector<E, base_type::Size> & rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.size(); i++)
			{
				tmp[i] = lhs[i] * static_cast<value_type>(rhs[i]);
			}
			return tmp;
		};
		
		template <class E>
		inline friend self_type operator/(const self_type & lhs, const Vector<E, base_type::Size> & rhs)
		{
			self_type tmp;
			for (size_t i = 0; i < tmp.size(); i++)
			{
				tmp[i] = lhs[i] / static_cast<value_type>(rhs[i]);
			}
			return tmp;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		template <class E>
		inline friend self_type & operator+=(self_type & lhs, const Vector<E, base_type::Size> & rhs)
		{
			return (lhs = (lhs + rhs));
		};
		
		template <class E>
		inline friend self_type & operator-=(self_type & lhs, const Vector<E, base_type::Size> & rhs)
		{
			return (lhs = (lhs - rhs));
		};
		
		template <class E>
		inline friend self_type & operator*=(self_type & lhs, const Vector<E, base_type::Size> & rhs)
		{
			return (lhs = (lhs * rhs));
		};
		
		template <class E>
		inline friend self_type & operator/=(self_type & lhs, const Vector<E, base_type::Size> & rhs)
		{
			return (lhs = (lhs / rhs));
		};

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VECTOR_HPP_