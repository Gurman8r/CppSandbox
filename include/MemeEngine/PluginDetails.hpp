#ifndef _ML_PLUGIN_DETAILS_HPP_
#define _ML_PLUGIN_DETAILS_HPP_

#include <MemeEngine/PluginAPI.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	struct ML_ENGINE_API PluginDetails final
		: public ITrackable
	{
		int32_t apiVersion;
		CString fileName;
		CString className;
		CString pluginName;
		CString pluginVersion;
	};
}

#endif // !_ML_PLUGIN_DETAILS_HPP_