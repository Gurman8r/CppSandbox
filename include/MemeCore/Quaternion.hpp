#ifndef _ML_QUATERNION_HPP_
#define _ML_QUATERNION_HPP_

#include <MemeCore/Vector4.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Quaternion final
		: public Vector4<float>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type	= typename float;
		using const_value	= typename const value_type;
		using base_type		= typename Vector4<value_type>;
		using complex_type	= typename Vector<value_type, 3>;

	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Quaternion()
			: Quaternion(1.0f)
		{
		}
		
		Quaternion(const_value xyz)
			: Quaternion(xyz, xyz, xyz)
		{
		}
		
		Quaternion(const_value x, const_value y, const_value z)
			: Quaternion(x, y, z, 0.0f)
		{
		}
		
		Quaternion(const_value x, const_value y, const_value z, const_value w)
			: base_type(x, y, z, w)
		{
		}
		
		Quaternion(const Vector<value_type, 4> & copy)
			: base_type(copy)
		{
		}
		
		Quaternion(const complex_type & xyz, const_value w)
			: base_type(xyz[0], xyz[1], xyz[2], w)
		{
		}
		
		Quaternion(const Quaternion & copy)
			: base_type((base_type)copy)
		{
		}
		
		~Quaternion() {}

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline complex_type complex() const
		{
			return (complex_type)(base_type)(*this);
		}

		inline value_type real() const
		{
			return this->back();
		}

		inline mat3f matrix() const
		{
			value_type xx = ((*this)[0] * (*this)[0]);
			value_type xy = ((*this)[0] * (*this)[1]);
			value_type xz = ((*this)[0] * (*this)[2]);
			value_type xw = ((*this)[0] * (*this)[3]);
			value_type yy = ((*this)[1] * (*this)[1]);
			value_type yz = ((*this)[1] * (*this)[2]);
			value_type yw = ((*this)[1] * (*this)[3]);
			value_type zz = ((*this)[2] * (*this)[2]);
			value_type zw = ((*this)[2] * (*this)[3]);

			return mat3f({
				(1.f - 2.f * yy - 2.f * zz), (2.f * xy - 2.f * zw), (2.f * xz + 2.f * yw),
				(2.f * xy + 2.f * zw), (1.f - 2.f * xx - 2.f * zz), (2.f * yz - 2.f * xw),
				(2.f * xz - 2.f * yw), (2.f * yz + 2.f * xw), (1.f - 2.f * xx - 2.f * yy)
			});
		}

		inline Quaternion & slerp(const Quaternion & other, const_value t)
		{
			return ((*this) = Quaternion::slerp((*this), other, t));
		}

		inline static Quaternion slerp(const Quaternion & lhs, const Quaternion & rhs, const_value t)
		{
			Quaternion a = lhs.normalized();
			Quaternion b = rhs.normalized();
			value_type d = a.dot(b);
			if (d > 0.9995f)
			{
				return (a + ((b - a) * t));
			}
			else if (d < 0.0f)
			{
				a = -a;
				d = -d;
			}
			d = Maths::clamp(d, -1.0f, 1.0f);
			value_type dt = (acosf(d) * t);
			return (b - a * d).normalized();
		}


	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * */
		inline friend Quaternion operator*(const Quaternion & lhs, const Quaternion & rhs)
		{
			return Quaternion(
			( lhs[0] * rhs[3]) + (lhs[1] * rhs[2]) - (lhs[2] * rhs[1]) + (lhs[3] * rhs[0]),
			(-lhs[0] * rhs[2]) + (lhs[1] * rhs[3]) + (lhs[2] * rhs[0]) + (lhs[3] * rhs[1]),
			( lhs[0] * rhs[1]) - (lhs[1] * rhs[0]) + (lhs[2] * rhs[3]) + (lhs[3] * rhs[2]),
			(-lhs[0] * rhs[0]) - (lhs[1] * rhs[1]) - (lhs[2] * rhs[2]) + (lhs[3] * rhs[3])
			);
		}
		
		inline friend Quaternion operator*(const Quaternion & lhs, value_type rhs)
		{
			return (base_type)(lhs) * rhs;
		}
		
		inline friend Quaternion operator/(const Quaternion & lhs, value_type rhs)
		{
			return (base_type)(lhs) / rhs;
		}
		
		inline operator base_type() const
		{
			return base_type((*this)[0], (*this)[1], (*this)[2], (*this)[3]);
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Types
	/* * * * * * * * * * * * * * * * * * * * */
	using quat = Quaternion;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_QUATERNION_HPP_