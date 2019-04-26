#ifndef _ML_TRANSFORM_HPP_
#define _ML_TRANSFORM_HPP_

#include <MemeCore/Quaternion.hpp>
#include <MemeCore/Rect.hpp>
#include <MemeCore/Vector2.hpp>
#include <MemeCore/Vector3.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Transform final
		: public ITrackable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Transform();
		Transform(const vec3f & pos, const vec3f & scl);
		Transform(const vec3f & pos, const vec3f & scl, const quat & rot);
		Transform(const mat4f & value);
		Transform(const Transform & copy);
		~Transform();


	public: // Static Functions
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static bool  Decompose(const mat4f & value, vec3f & scl, quat & rot, vec3f & tns, vec3f & skw, vec4f & psp);
		static mat4f Rotate(const mat4f & value, const float angle, const vec3f & axis);
		static mat4f Rotate(const mat4f & value, const quat & rot);
		static mat4f Scale(const mat4f & value, const vec3f & scl);
		static mat4f Translate(const mat4f & value, const vec3f & tns);
		static mat4f LookAt(const vec3f & eye, const vec3f & pos, const vec3f & up);


	public: // Member Functions
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Transform &	rotate(const float angle, const vec3f & axis);
		Transform & rotate(const quat & value);
		Transform &	scale(const vec3f & value);
		Transform &	translate(const vec3f & value);
		Transform & lookAt(const vec3f & eye, const vec3f & pos, const vec3f & up);

		/* * * * * * * * * * * * * * * * * * * * */

		Transform & update(const Transform & other);// Set Matrix
		Transform & update(const mat4f & value);	// Set Matrix

		/* * * * * * * * * * * * * * * * * * * * */

		bool decompose(vec3f & scl, quat & rot, vec3f & tns, vec3f & skw, vec4f & psp) const;
		const Transform & decompose() const;

		/* * * * * * * * * * * * * * * * * * * * */

		inline const mat4f & getMatrix() const		// Get Matrix
		{ 
			return m_matrix; 
		}

		const vec4f	& getPsp() const;				// Get Perspective
		const vec3f	& getPos() const;				// Get Position
		const quat	& getRot() const;				// Get Rotation
		const vec3f	& getScl() const;				// Get Scale
		const vec3f	& getSkw() const;				// Get Skew

	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		mat4f m_matrix;

		mutable bool	m_changed	= true;
		mutable vec4f	m_psp		= 0.0f;			// Perspective
		mutable vec3f	m_pos		= 0.0f;			// Position
		mutable quat	m_rot		= 1.0f;			// Rotation
		mutable vec3f	m_scl		= 0.0f;			// Scale
		mutable vec3f	m_skw		= 0.0f;			// Skew
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TRANSFORM_HPP_