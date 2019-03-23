#include <MemeEditor/Inspector.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Inspector::Inspector()
	{
	}

	Inspector::~Inspector()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Inspector::onEvent(const IEvent * value)
	{
	}

	bool Inspector::draw(CString title, bool * p_open)
	{
		return beginDraw(title, p_open, ImGuiWindowFlags_AlwaysAutoResize);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}