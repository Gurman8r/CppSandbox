#ifndef _ML_UNI_HPP_
#define _ML_UNI_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Texture;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class uni_base
		: public ITrackable
		, public INonCopyable
	{
	public:
		enum Type : int32_t
		{
			None,
			Int,
			Float,
			Vec2,
			Vec3,
			Vec4, Col4,
			Mat3,
			Mat4,
			Tex2D,
			MAX_TYPE
		};

		String	name;
		Type	type;

		uni_base(const String & name, const Type type)
			: name(name)
			, type(type)
		{
		}

		virtual ~uni_base() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Elem, uni_base::Type Ty>
	struct uni_t : public uni_base
	{
		static constexpr auto MyType { (Type)Ty };

		Elem data;

		uni_t(const String & name, Elem data)
			: uni_base(name, Ty)
			, data(data)
		{
		}

		virtual ~uni_t() {}
	};

	template <class Elem, uni_base::Type Ty>
	struct uni_cr_t : uni_t<const Elem &, Ty>
	{
		uni_cr_t(const String & name, const Elem & data)
			: uni_t<const Elem &, Ty>(name, data)
		{
		}

		virtual ~uni_cr_t() {}
	};

	template <class Elem, uni_base::Type Ty>
	struct uni_cp_t : uni_t<const Elem *, Ty>
	{
		uni_cp_t(const String & name, const Elem * data)
			: uni_t<const Elem *, Ty>(name, data)
		{
		}

		virtual ~uni_cp_t() {}
	};

	// Value
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	using uni_flt		= typename uni_t<float,			uni_base::Float>;
	using uni_int		= typename uni_t<int32_t,		uni_base::Int>;
	using uni_vec2		= typename uni_t<vec2,			uni_base::Vec2>;
	using uni_vec3		= typename uni_t<vec3,			uni_base::Vec3>;
	using uni_vec4		= typename uni_t<vec4,			uni_base::Vec4>;
	using uni_col		= typename uni_t<vec4,			uni_base::Col4>;
	using uni_mat3		= typename uni_t<mat3,			uni_base::Mat3>;
	using uni_mat4		= typename uni_t<mat4,			uni_base::Mat4>;

	// Const Reference
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	using uni_cr_flt	= typename uni_cr_t<float,		uni_base::Float>;
	using uni_cr_int	= typename uni_cr_t<int32_t,	uni_base::Int>;
	using uni_cr_vec2	= typename uni_cr_t<vec2,		uni_base::Vec2>;
	using uni_cr_vec3	= typename uni_cr_t<vec3,		uni_base::Vec3>;
	using uni_cr_vec4	= typename uni_cr_t<vec4,		uni_base::Vec4>;
	using uni_cr_col	= typename uni_cr_t<vec4,		uni_base::Col4>;
	using uni_cr_mat3	= typename uni_cr_t<mat3,		uni_base::Mat3>;
	using uni_cr_mat4	= typename uni_cr_t<mat4,		uni_base::Mat4>;

	// Const Pointer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	using uni_cp_flt	= typename uni_cp_t<float,		uni_base::Float>;
	using uni_cp_int	= typename uni_cp_t<int32_t,	uni_base::Int>;
	using uni_cp_vec2	= typename uni_cp_t<vec2,		uni_base::Vec2>;
	using uni_cp_vec3	= typename uni_cp_t<vec3,		uni_base::Vec3>;
	using uni_cp_vec4	= typename uni_cp_t<vec4,		uni_base::Vec4>;
	using uni_cp_col	= typename uni_cp_t<vec4,		uni_base::Col4>;
	using uni_cp_mat3	= typename uni_cp_t<mat3,		uni_base::Mat3>;
	using uni_cp_mat4	= typename uni_cp_t<mat4,		uni_base::Mat4>;
	using uni_cp_tex	= typename uni_cp_t<Texture,	uni_base::Tex2D>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNI_HPP_