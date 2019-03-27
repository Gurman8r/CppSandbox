#include <MemeEditor/Profiler.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Profiler::Profiler()
		: GUI_Window("Profiler")
	{
	}

	Profiler::~Profiler()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Profiler::onEvent(const IEvent * value)
	{
	}

	bool Profiler::draw(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("%s (WIP)", title());
				ImGui::EndMenuBar();
			}
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}