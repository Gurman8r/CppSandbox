#include <MemeEditor/Inspector.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Inspector::Inspector()
		: base_type("Inspector")
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
		return beginDraw(p_open, ImGuiWindowFlags_AlwaysAutoResize);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}