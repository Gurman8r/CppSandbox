#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>

#define ML_Engine ml::Engine::getInstance()

namespace ml
{
	class ML_ENGINE_API Engine final
		: public ITrackable
		, public ISingleton<Engine>
	{
		friend class ISingleton<Engine>;

	private:
		Engine();
		~Engine();

	public:

	};
}

#endif // !_ML_ENGINE_HPP_