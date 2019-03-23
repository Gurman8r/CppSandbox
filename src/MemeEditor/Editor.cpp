#include <MemeEditor/Editor.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	Editor::Editor()
	{
	}

	Editor::~Editor()
	{
	}

	void Editor::onEvent(const IEvent * value)
	{
	}

	bool Editor::beginWindow(CString title, bool * p_open, int32_t flags)
	{
		if (m_good = (ImGui::Begin(title, p_open, flags)))
		{
			
		}
		return m_good;
	}

	bool Editor::endWindow()
	{
		if (m_good)
		{

		}
		ImGui::End();
		return m_good;
	}
}