#ifndef _ML_QUAT_HPP_
#define _ML_QUAT_HPP_

#include <MemeCore/Vector4.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Quat final
		: public Vector<float, 4>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type			= typename value_type;
		using self_type				= typename Quat;
		using base_type				= typename Vector<value_type, 4>;
		using complex_type			= typename Vector<value_type, 3>;

		using pointer				= typename base_type::pointer;
		using reference				= typename base_type::reference;
		using const_pointer			= typename base_type::const_pointer;
		using const_reference		= typename base_type::const_reference;

		using iterator				= typename base_type::iterator;
		using const_iterator		= typename base_type::const_iterator;
		using reverse_iterator		= typename base_type::reverse_iterator;
		using const_reverse_iterator= typename base_type::const_reverse_iterator;


	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Quat()
			: self_type(1.0f)
		{
		}
		
		Quat(const_reference xyz)
			: self_type(xyz, xyz, xyz)
		{
		}
		
		Quat(const_reference x, const_reference y, const_reference z)
			: self_type(x, y, z, 0.0f)
		{
		}
		
		Quat(const_reference x, const_reference y, const_reference z, const_reference w)
			: base_type({ x, y, z, w })
		{
		}
		
		Quat(const complex_type & xyz, const_reference w)
			: base_type({ xyz[0], xyz[1], xyz[2], w })
		{
		}

		template <
			size_t S
		> Quat(const Vector<value_type, S> & copy)
			: base_type(copy)
		{
		}

		template <
			class T
		> Quat(const Vector4<T> & copy)
			: base_type(copy)
		{
		}

		template <
			class T,
			size_t S
		> Quat(const Vector<T, S> & copy, const_reference def = static_cast<value_type>(0))
			: base_type(copy, def)
		{
		}
		
		~Quat() {}


	public: // Member Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline complex_type complex() const { return (complex_type)(*this); }

		inline const_reference real() const { return this->back(); }

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
		

	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * */
		inline operator vec4f() const
		{
			return vec4f((*this)[0], (*this)[1], (*this)[2], (*this)[3]);
		}

		inline friend Quat operator*(const Quat & lhs, const Quat & rhs)
		{
			return Quat(
				( lhs[0] * rhs[3]) + (lhs[1] * rhs[2]) - (lhs[2] * rhs[1]) + (lhs[3] * rhs[0]),
				(-lhs[0] * rhs[2]) + (lhs[1] * rhs[3]) + (lhs[2] * rhs[0]) + (lhs[3] * rhs[1]),
				( lhs[0] * rhs[1]) - (lhs[1] * rhs[0]) + (lhs[2] * rhs[3]) + (lhs[3] * rhs[2]),
				(-lhs[0] * rhs[0]) - (lhs[1] * rhs[1]) - (lhs[2] * rhs[2]) + (lhs[3] * rhs[3])
			);
		}

		inline friend Quat operator*(const Quat & lhs, value_type rhs)
		{
			return Quat();
		}

		inline friend Quat operator/(const Quat & lhs, value_type rhs)
		{
			return Quat();
		}

# ifdef GLM_VERSION
	public: // GLM
		/* * * * * * * * * * * * * * * * * * * * */
		Quat(const glm::tquat<float, glm::defaultp> & value)
			: self_type(value.x, value.y, value.z, value.w)
		{
		}

		inline operator glm::tquat<float, glm::defaultp>() const
		{
			return glm::tquat<float, glm::defaultp>(
				(*this)[0], (*this)[1], (*this)[2], (*this)[3]
			);
		}
# endif
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_QUAT_HPP_