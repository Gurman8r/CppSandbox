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
		using value_type		= typename float;
		using self_type			= typename Quaternion;
		using base_type			= typename Vector4<value_type>;
		using complex_type		= typename Vector<value_type, 3>;

		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;


	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Quaternion()
			: self_type(1.0f)
		{
		}
		
		Quaternion(const_reference xyz)
			: self_type(xyz, xyz, xyz)
		{
		}
		
		Quaternion(const_reference x, const_reference y, const_reference z)
			: self_type(x, y, z, 0.0f)
		{
		}
		
		Quaternion(const_reference x, const_reference y, const_reference z, const_reference w)
			: base_type(x, y, z, w)
		{
		}
		
		Quaternion(const Vector<value_type, 4> & copy)
			: base_type(copy)
		{
		}
		
		Quaternion(const complex_type & xyz, const_reference w)
			: base_type(xyz[0], xyz[1], xyz[2], w)
		{
		}
		
		Quaternion(const self_type & copy)
			: base_type((base_type)copy)
		{
		}
		
		~Quaternion() {}


	public: // Member Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline complex_type complex() const
		{
			return (complex_type)(base_type)(*this);
		}

		inline const_reference real() const
		{
			return this->back();
		}

		inline mat3f rotationMatrix() const
		{
			const value_type xx = ((*this)[0] * (*this)[0]);
			const value_type xy = ((*this)[0] * (*this)[1]);
			const value_type xz = ((*this)[0] * (*this)[2]);
			const value_type xw = ((*this)[0] * (*this)[3]);
			const value_type yy = ((*this)[1] * (*this)[1]);
			const value_type yz = ((*this)[1] * (*this)[2]);
			const value_type yw = ((*this)[1] * (*this)[3]);
			const value_type zz = ((*this)[2] * (*this)[2]);
			const value_type zw = ((*this)[2] * (*this)[3]);

			return mat3f({
				(1.f - 2.f * yy - 2.f * zz), (2.f * xy - 2.f * zw), (2.f * xz + 2.f * yw),
				(2.f * xy + 2.f * zw), (1.f - 2.f * xx - 2.f * zz), (2.f * yz - 2.f * xw),
				(2.f * xz - 2.f * yw), (2.f * yz + 2.f * xw), (1.f - 2.f * xx - 2.f * yy)
			});
		}

		inline self_type & slerp(const self_type & other, const_reference t)
		{
			return ((*this) = self_type::slerp((*this), other, t));
		}


	public: // Static Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline static self_type slerp(const self_type & lhs, const self_type & rhs, const_reference t)
		{
			self_type a = lhs.normalized();
			self_type b = rhs.normalized();
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
		inline operator base_type() const
		{
			return base_type((*this)[0], (*this)[1], (*this)[2], (*this)[3]);
		}

		inline friend self_type operator*(const self_type & lhs, const self_type & rhs)
		{
			return self_type(
				( lhs[0] * rhs[3]) + (lhs[1] * rhs[2]) - (lhs[2] * rhs[1]) + (lhs[3] * rhs[0]),
				(-lhs[0] * rhs[2]) + (lhs[1] * rhs[3]) + (lhs[2] * rhs[0]) + (lhs[3] * rhs[1]),
				( lhs[0] * rhs[1]) - (lhs[1] * rhs[0]) + (lhs[2] * rhs[3]) + (lhs[3] * rhs[2]),
				(-lhs[0] * rhs[0]) - (lhs[1] * rhs[1]) - (lhs[2] * rhs[2]) + (lhs[3] * rhs[3])
			);
		}

		inline friend self_type operator*(const self_type & lhs, value_type rhs)
		{
			return ((base_type)(lhs)* rhs);
		}

		inline friend self_type operator/(const self_type & lhs, value_type rhs)
		{
			return ((base_type)(lhs) / rhs);
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