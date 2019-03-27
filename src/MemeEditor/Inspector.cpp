#include <MemeEditor/Inspector.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeEditor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Inspector::Inspector()
		: GUI_Window("Inspector")
	{
		ML_EventSystem.addListener(EditorEvent::EV_MainMenuBar, this);
	}

	Inspector::~Inspector()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Inspector::onEvent(const IEvent * value)
	{
		switch (value->eventID())
		{
		case EditorEvent::EV_MainMenuBar:
			if (const auto * ev = value->as<MainMenuBarEvent>())
			{
			}
			break;
		}
	}

	bool Inspector::draw(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("%s (WIP)", ML_Inspector.title());
				ImGui::EndMenuBar();
			}
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}