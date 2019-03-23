#include <MemeEditor/EditorBase.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	EditorBase::EditorBase()
		: m_good (false)
		, m_title(NULL)
		, m_open (NULL)
		, m_flags(0)
	{
	}

	EditorBase::~EditorBase()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool EditorBase::beginDraw(CString title, bool * p_open, int32_t flags)
	{
		return m_good = ImGui::Begin(
			(m_title = title),
			(m_open = p_open),
			(m_flags = flags));
	}

	bool EditorBase::endDraw()
	{
		ImGui::End();
		return m_good;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t EditorBase::getID() const
	{
		return ImGui::GetID(m_title);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}