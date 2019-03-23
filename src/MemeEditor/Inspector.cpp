#include <MemeEditor/Inspector.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeEditor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Inspector::Inspector()
		: base_type("Inspector")
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
		return beginDraw(p_open, ImGuiWindowFlags_AlwaysAutoResize);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}