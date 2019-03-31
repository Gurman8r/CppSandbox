#ifndef _ML_ENGINE_SETTINGS_HPP_
#define _ML_ENGINE_SETTINGS_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_ENGINE_API EngineSettings final
		: public ITrackable
	{
	public:
		EngineSettings();
		~EngineSettings();

	private:

	};
}

#endif // !_ML_ENGINE_SETTINGS_HPP_