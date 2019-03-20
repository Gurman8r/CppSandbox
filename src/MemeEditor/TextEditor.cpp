#include <MemeEditor/TextEditor.hpp>
#include <MemeEditor/Terminal.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_ml.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	TextEditor::TextEditor()
		: TextEditor("Test")
	{
	}

	TextEditor::TextEditor(const String & text)
		: TextEditor(text.data(), text.size())
	{
	}

	TextEditor::TextEditor(CString text, size_t size)
		: m_selected(-1)
	{
		m_files.push_back(Document("Test", text, size));
	}

	TextEditor::~TextEditor()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void TextEditor::draw(CString label, bool * p_open)
	{
		if (ImGui::Begin(label, p_open, 
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_AlwaysHorizontalScrollbar |
			ImGuiWindowFlags_AlwaysVerticalScrollbar))
		{
			draw_menu();
			
			draw_tabs();
		}
		ImGui::End();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void TextEditor::draw_menu()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", NULL, false, true))
				{
					m_files.push_back(Document(
						String("New_File ({0})").format(m_files.size()).c_str()
					));
				}


				
				if (ImGui::MenuItem("Save", NULL, false, true))
				{
					if (Document * doc = get_selected_doc())
					{
						doc->dirty = false;
					}
				}

				ImGui::EndMenu();
			}

			ImGui::Text("( %d/%u )", (m_selected + 1), m_files.size());

			ImGui::EndMenuBar();
		}
	}

	void TextEditor::draw_tabs()
	{
		if (ImGui::BeginTabBar("Tabs", ImGuiTabBarFlags_Reorderable))
		{
			m_selected = -1;
			for (size_t i = 0; i < m_files.size(); i++)
			{
				if (draw_document((int32_t)i))
				{
					m_selected = (int32_t)i;
				}
			}
			ImGui::EndTabBar();
		}
	}

	bool TextEditor::draw_document(int32_t index)
	{
		if (Document * doc = get_doc((size_t)index))
		{
			ImGuiTabItemFlags flags = (doc->dirty ? ImGuiTabItemFlags_UnsavedDocument : 0);

			if (ImGui::BeginTabItem(doc->name, &doc->open, flags))
			{
				Document::Name buf;
				strcpy(buf, doc->name);
				if (ImGui::InputText(
					("Name"),
					(buf),
					(Document::NameSize),
					(ImGuiInputTextFlags_EnterReturnsTrue),
					(NULL), 
					(NULL)))
				{
					strcpy(doc->name, buf);
				}

				ImGui::Separator();

				if (ImGui::InputTextMultiline(
					("##Data"),
					(doc->data),
					(Document::DataSize),
					(ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16)),
					(ImGuiInputTextFlags_AllowTabInput),
					(NULL),
					(NULL)))
				{
					doc->dirty = true;
				}

				ImGui::Separator();

				ImGui::Text("%u/%u", doc->sizeUsed(), doc->sizeMax());
				ImGui::Text("Data:\n\"%s\"", String(doc->data).c_str());

				ImGui::EndTabItem();

				return true;
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}