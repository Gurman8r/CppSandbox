#ifndef _ML_IMGUI_BUILTIN_HPP_
#define _ML_IMGUI_BUILTIN_HPP_

#include <MemeEditor/Export.hpp>

namespace ml
{
	// ImGui Demo Windows
	class ML_EDITOR_API ImGui_Builtin final
	{
	public:
		static void showDemoWindow(bool & p_open);
		static void showMetricsWindow(bool & p_open);
		static void showStyleWindow(bool & p_open);
		static void showAboutWindow(bool & p_open);
	};
}

#endif // !_ML_IMGUI_BUILTIN_HPP_