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

	// Entity Component System (WIP)
	class ML_ENGINE_API ECS final
		: public ITrackable
		, public ISingleton<ECS>
	{
		friend class ISingleton<ECS>;

	private:
		ECS();
		~ECS();

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ECS_HPP_