#ifndef _IMGUI_BUILTIN_HPP_
#define _IMGUI_BUILTIN_HPP_

#include <MemeEditor/Export.hpp>

namespace ml
{
	// Wrappers for builtin ImGui functions
	class ML_EDITOR_API ImGuiBuiltin final
	{
	public:
		static void showDemoWindow(bool & p_open);
		static void showMetricsWindow(bool & p_open);
		static void showStyleWindow(bool & p_open);
		static void showAboutWindow(bool & p_open);
	};
}

#endif // !_IMGUI_BUILTIN_HPP_
