#include <MemeEditor/GUIDrawer.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	GUIDrawer::GUIDrawer()
		: m_good (false)
		, m_title(NULL)
		, m_open (NULL)
		, m_flags(0)
	{
	}

	GUIDrawer::~GUIDrawer()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool GUIDrawer::beginDraw(CString title, bool * p_open, int32_t flags)
	{
		return m_good = ImGui::Begin(
			(m_title = title),
			(m_open = p_open),
			(m_flags = flags));
	}

	bool GUIDrawer::endDraw()
	{
		ImGui::End();
		return m_good;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t GUIDrawer::getID() const
	{
		return ImGui::GetID(m_title);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}