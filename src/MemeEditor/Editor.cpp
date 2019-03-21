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

	bool Editor::beginWindow(CString title, bool * p_open, int32_t flags)
	{
		if (m_good = (ImGui::Begin(title, p_open, flags)))
		{
			
		}
		return m_good;
	}

	void Editor::endWindow()
	{
		if (m_good)
		{

		}
		ImGui::End();
	}

	bool Editor::draw(CString title, bool * p_open)
	{
		if (beginWindow(title, p_open))
		{
			endWindow();
			return true;
		}
		return false;
	}
}