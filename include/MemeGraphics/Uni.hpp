#ifndef _ML_UNI_HPP_
#define _ML_UNI_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Texture;

	// Base Uni
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	class uni_base
		: public ITrackable
		, public INonCopyable
	{
	public:
		using type_base = typename int32_t;
		using id_type	= typename const type_base;

	public:
		enum : type_base
		{
			None,
			Int,	// Integer
			Flt,	// Float
			Vec2,	// Vector2
			Vec3,	// Vector3
			Vec4,	// Vector4
			Col4,	// Color
			Mat3,	// Matrix3x3
			Mat4,	// Matrix4x4
			Tex,	// Texture
			MAX_TYPE
		};

	public:
		String	name;
		id_type type;

	public:
		uni_base(const String & name, id_type type)
			: name(name)
			, type(type)
		{
		}

		virtual ~uni_base() {}
	};

	// Base Generic Uni
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <
		class	_Elem, 
		int32_t _Type
	> struct uni_t : public uni_base
	{
		using value_type	= typename _Elem;
		using base_type		= typename uni_base;
		using self_type		= typename uni_t<value_type, _Type>;

		enum : id_type { ID = _Type };

		value_type data;

		uni_t(const String & name, value_type data) : base_type(name, ID), data(data) {}
	};


	// Generators
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_GEN_UNI_T(NAME, TYPE)											\
	template <class T> struct NAME : public uni_t<T, TYPE>					\
	{																		\
		using value_type	= typename T;									\
		using base_type		= typename uni_t<T, TYPE>;						\
		using self_type		= typename NAME<value_type>;					\
		NAME(const String & name, T data) : uni_t<T, TYPE>(name, data) {}	\
	};

	/* * * * * * * * * * * * * * * * * * * * */

	ML_GEN_UNI_T(	uni_flt_t,		uni_base::Flt	);
	ML_GEN_UNI_T(	uni_int_t,		uni_base::Int	);
	ML_GEN_UNI_T(	uni_vec2_t,		uni_base::Vec2	);
	ML_GEN_UNI_T(	uni_vec3_t,		uni_base::Vec3	);
	ML_GEN_UNI_T(	uni_vec4_t,		uni_base::Vec4	);
	ML_GEN_UNI_T(	uni_col4_t,		uni_base::Col4	);
	ML_GEN_UNI_T(	uni_mat3_t,		uni_base::Mat3	);
	ML_GEN_UNI_T(	uni_mat4_t,		uni_base::Mat4	);
	ML_GEN_UNI_T(	uni_tex_t,		uni_base::Tex	);


	// Value Types
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	using uni_flt		= typename uni_flt_t	<float>;
	using uni_int		= typename uni_int_t	<int32_t>;
	using uni_vec2		= typename uni_vec2_t	<vec2>;
	using uni_vec3		= typename uni_vec3_t	<vec3>;
	using uni_vec4		= typename uni_vec4_t	<vec4>;
	using uni_col4		= typename uni_col4_t	<vec4>;
	using uni_mat3		= typename uni_mat3_t	<mat3>;
	using uni_mat4		= typename uni_mat4_t	<mat4>;


	// Const Reference Types
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	using uni_cr_flt	= typename uni_flt_t	<const float &>;
	using uni_cr_int	= typename uni_int_t	<const int32_t &>;
	using uni_cr_vec2	= typename uni_vec2_t	<const vec2 &>;
	using uni_cr_vec3	= typename uni_vec3_t	<const vec3 &>;
	using uni_cr_vec4	= typename uni_vec3_t	<const vec4 &>;
	using uni_cr_col4	= typename uni_col4_t	<const vec4 &>;
	using uni_cr_mat3	= typename uni_mat3_t	<const mat3 &>;
	using uni_cr_mat4	= typename uni_mat4_t	<const mat4 &>;


	// Const Pointer Types
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	using uni_cp_flt	= typename uni_flt_t	<const float *>;
	using uni_cp_int	= typename uni_int_t	<const int32_t *>;
	using uni_cp_vec2	= typename uni_vec2_t	<const vec2 *>;
	using uni_cp_vec3	= typename uni_vec3_t	<const vec3 *>;
	using uni_cp_vec4	= typename uni_vec3_t	<const vec4 *>;
	using uni_cp_col4	= typename uni_col4_t	<const vec4 *>;
	using uni_cp_mat3	= typename uni_mat3_t	<const mat3 *>;
	using uni_cp_mat4	= typename uni_mat4_t	<const mat4 *>;
	using uni_cp_tex	= typename uni_tex_t	<const Texture *>;
}

#endif // !_ML_UNI_HPP_