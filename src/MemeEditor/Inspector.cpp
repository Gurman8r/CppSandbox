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

	bool Inspector::drawGui(bool * p_open)
	{
		return beginDraw(p_open, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}