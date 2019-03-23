#include <MemeEditor/GUI_Window.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	GUI_Window::GUI_Window()
		: GUI_Base()
		, m_title(NULL)
		, m_open (NULL)
		, m_flags(ImGuiWindowFlags_None)
	{
	}
	GUI_Window::~GUI_Window()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t GUI_Window::getID() const
	{
		return ImGui::GetID(m_title);
	}

	bool GUI_Window::beginDraw(CString title, bool * p_open, int32_t flags)
	{
		return goodCheck(ImGui::Begin(
			(m_title = title),
			(m_open = p_open),
			(m_flags = flags))
		);
	}

	bool GUI_Window::endDraw()
	{
		ImGui::End();
		return good();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}