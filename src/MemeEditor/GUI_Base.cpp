#include <MemeEditor/GUI_Base.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	GUI_Base::GUI_Base()
		: m_good (false)
		, m_title(NULL)
		, m_open (NULL)
		, m_flags(ImGuiWindowFlags_None)
	{
	}

	GUI_Base::~GUI_Base()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool GUI_Base::beginDraw(CString title, bool * p_open, int32_t flags)
	{
		return m_good = ImGui::Begin(
			(m_title = title),
			(m_open = p_open),
			(m_flags = flags));
	}

	bool GUI_Base::endDraw()
	{
		ImGui::End();
		return m_good;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t GUI_Base::getID() const
	{
		return ImGui::GetID(m_title);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}