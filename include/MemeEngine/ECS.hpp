#ifndef _ML_ECS_HPP_
#define _ML_ECS_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeEngine/Entity.hpp>
#include <MemeGraphics/Transform.hpp>
#include <MemeCore/Detection.hpp>

#define ML_ECS ml::ECS::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Transform Component (example)
	struct Cmp_Transform
	{
		vec3f	position;
		vec3f	scale;
		quat	rotation;
	};

	// Rigidbody Component (example)
	struct Cmp_Rigidbody
	{
		vec3f	velocity;
		vec3f	acceleration;
		float	mass;
		float	massInv;
	};

	// Movement System (example)
	struct Sys_Movement
	{
		void Update(float dt, Cmp_Transform * tf, Cmp_Rigidbody * rb)
		{
			tf->position += rb->velocity * dt;
			rb->velocity += rb->acceleration * dt;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	// Entity Component System (WIP)
	class ML_ENGINE_API ECS final
		: public ITrackable
		, public ISingleton<ECS>
	{
		friend class ISingleton<ECS>;

	private:
		ECS();
		~ECS();

	public:
		Sys_Movement movement;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ECS_HPP_