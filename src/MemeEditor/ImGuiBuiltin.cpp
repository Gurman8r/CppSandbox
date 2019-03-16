#include <MemeEditor/ImGuiBuiltin.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_ml.hpp>

namespace ml
{
	void ImGuiBuiltin::showDemoWindow(bool & p_open)
	{
		if (p_open) { ImGui::ShowDemoWindow(&p_open); }
	}
	void ImGuiBuiltin::showMetricsWindow(bool & p_open)
	{
		if (p_open) { ImGui::ShowMetricsWindow(&p_open); }
	}
	void ImGuiBuiltin::showStyleWindow(bool & p_open)
	{
		if (p_open) 
		{ 
			ImGui::Begin("Style Editor", &p_open); 
			ImGui::ShowStyleEditor(); 
			ImGui::End(); 
		}
	}
	void ImGuiBuiltin::showAboutWindow(bool & p_open)
	{
		if (p_open) { ImGui::ShowAboutWindow(&p_open); }
	}
}