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
	}

	Inspector::~Inspector()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Inspector::onEvent(const IEvent * value)
	{
	}

	bool Inspector::draw(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ML_EventSystem.fireEvent(InspectorEvent());
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}