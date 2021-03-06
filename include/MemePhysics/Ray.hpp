#ifndef _ML_RAY_HPP_
#define _ML_RAY_HPP_

#include <MemePhysics/Export.hpp>
#include <MemeCore/Vector3.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API Ray final
		: public ITrackable
	{
	public:
		Ray();
		Ray(const vec3 & origin, const vec3 & direction);
		Ray(const Ray & copy);
		~Ray();

	public:
		inline const vec3 & origin()	const	{ return m_origin; }
		inline vec3 &		origin()			{ return m_origin; }

		inline const vec3 & direction() const	{ return m_direction; }
		inline vec3 &		direction()			{ return m_direction; }

	private:
		vec3 m_origin;
		vec3 m_direction;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RAY_HPP_