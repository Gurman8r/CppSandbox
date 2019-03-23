#include <MemeEditor/ImGui_Builtin.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	void ImGui_Builtin::showDemoWindow(bool & p_open)
	{
		if (p_open) { ImGui::ShowDemoWindow(&p_open); }
	}
	
	void ImGui_Builtin::showMetricsWindow(bool & p_open)
	{
		if (p_open) { ImGui::ShowMetricsWindow(&p_open); }
	}
	
	void ImGui_Builtin::showStyleWindow(bool & p_open)
	{
		if (p_open) 
		{ 
			ImGui::Begin("Style Editor", &p_open); 
			ImGui::ShowStyleEditor(); 
			ImGui::End(); 
		}
	}
	
	void ImGui_Builtin::showAboutWindow(bool & p_open)
	{
		if (p_open) { ImGui::ShowAboutWindow(&p_open); }
	}
}