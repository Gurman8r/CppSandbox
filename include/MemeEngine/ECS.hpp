#ifndef _ML_ECS_HPP_
#define _ML_ECS_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeEngine/IEntity.hpp>
#include <MemeGraphics/Transform.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_ECS ml::ECS::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Position final
	{
		vec3f value;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <typename TComponent>
	struct ECS_Settings
	{
	};

	class ML_ENGINE_API ECS final
		: public ITrackable
		, public ISingleton<ECS>
	{
		friend class ISingleton<ECS>;

	public:
		enum { MaxEntity = 10 };

	private:
		ECS();
		~ECS();

	public:
		using TypeInfo = typename const std::type_info *;

		template <typename T>
		using ComponentArray = typename std::array<T, MaxEntity>;

		using ComponentMap = typename HashMap<TypeInfo, std::array<void *, MaxEntity>>;

	private:
		ComponentMap m_map;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ECS_HPP_