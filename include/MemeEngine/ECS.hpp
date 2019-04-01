#ifndef _ML_ECS_HPP_
#define _ML_ECS_HPP_

#include <MemeEngine/IEntity.hpp>

#define ML_ECS ml::ECS::getInstance()

namespace ml
{
	class ML_ENGINE_API ECS final
		: public ITrackable
		, public ISingleton<ECS>
	{
		friend class ISingleton<ECS>;

	private:
		ECS();
		~ECS();

	public:

	};
}

#endif // !_ML_ECS_HPP_