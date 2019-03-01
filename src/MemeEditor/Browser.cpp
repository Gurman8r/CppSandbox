#include <MemeEditor/Browser.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeCore/OS.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_ml.hpp>



namespace ml
{
	Browser::Browser()
		: m_path	()
		, m_dir		()
		, m_type	(T_Dir)
		, m_index	(-1)
		, m_preview	()
		, m_isDouble(false)
	{
		ML_EventSystem.addListener(CoreEvent::EV_FileSystem, this);

		onEvent(&FileSystemEvent());
	}

	Browser::~Browser()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Browser::onEvent(const IEvent * value)
	{
		switch (value->eventID())
		{
		case CoreEvent::EV_FileSystem:
			if (const auto * ev = value->as<FileSystemEvent>())
			{
				m_path = ML_FileSystem.getWorkingDir();
				if (ML_FileSystem.getDirContents(m_path, m_dir))
				{
					set_selected(T_Unk, -1);
				}
			}
			break;
		}
	}

	void Browser::draw(bool * p_open)
	{
		ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Browser", (m_open = p_open), ImGuiWindowFlags_MenuBar))
		{
			ImGui::End();
			return;
		}
		else
		{
			draw_menu();

			draw_directory();

			ImGui::SameLine();

			draw_file();

			// Handle Double Clicks
			if (m_isDouble)
			{
				m_isDouble = false;
				switch (m_type)
				{
				case T_Dir: ML_FileSystem.setWorkingDir(get_selected_name()); break;
				}
			}
			
		}
		ImGui::End();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Browser::draw_menu()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Close")) { (*m_open) = false; }

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text("%s", m_path.c_str());
	}

	void Browser::draw_directory()
	{
		ImGui::BeginChild("Directory View", { 256, 0 }, true);
		{
			m_isDouble = false;

			ImVec4 col_default_text = ImGui::GetStyleColorVec4(ImGuiCol_Text);

			for (const Pair<char, List<String>> & pair : m_dir)
			{
				const char & type = pair.first;
				const List<String> & list = pair.second;

				ImVec4 col;
				switch (type)
				{
				case T_Dir	: col = ImColor(0.0f, 0.4f, 1.0f, 1.0f); break;
				case T_Reg	: col = ImColor(0.0f, 1.0f, 0.4f, 1.0f); break;
				case T_Lnk	: col = ImColor(0.0f, 1.0f, 1.0f, 0.0f); break;
				default		: col = (col_default_text); break;
				}

				ImGui::PushStyleColor(ImGuiCol_Text, col);
				for (size_t i = 0, imax = list.size(); i < imax; i++)
				{
					const String & name = list.at(i);

					if (ImGui::Selectable(
						((name + type).c_str()),
						((m_type == type) && ((size_t)m_index == i)),
						(ImGuiSelectableFlags_AllowDoubleClick)))
					{
						if (ImGui::IsMouseDoubleClicked(0))
						{
							(m_isDouble = true); break;
						}
						else
						{
							set_selected(pair.first, (int32_t)i);
						}
					}
				}
				ImGui::PopStyleColor();
				ImGui::Separator();

				if (m_isDouble) break;
			}
		}
		ImGui::EndChild();
	}

	void Browser::draw_file()
	{
		ImGui::BeginGroup();
		{
			ImGui::BeginChild("Content View", { 0, -ImGui::GetFrameHeightWithSpacing() });
			{
				ImGui::Separator();

				if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
				{
					draw_file_preview();
					
					draw_file_details();
					
					ImGui::EndTabBar();
				}
			}
			ImGui::EndChild();

			ImGui::Separator();

			if (ImGui::Button("Open"))
			{
				ML_OS.execute("open", this->pathTo(get_selected_name()));
			}
		}
		ImGui::EndGroup();
	}

	void Browser::draw_file_preview()
	{
		if (ImGui::BeginTabItem("Preview"))
		{
			switch (m_type)
			{
			case T_Dir:
			case T_Reg: 
				ImGui::TextWrapped("%s", (CString)m_preview); 
				break;

			default: ImGui::TextWrapped("%s", get_selected_name().c_str()); break;
			}
			ImGui::EndTabItem();
		}
	}

	void Browser::draw_file_details()
	{
		if (ImGui::BeginTabItem("Details"))
		{
			ImGui::Text("Name: %s", get_selected_name().c_str());
			ImGui::Text("Type: %s", get_selected_ext().c_str());
			ImGui::Text("Size: %u (B)", ML_FileSystem.getFileSize(get_selected_name()));
			ImGui::EndTabItem();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Browser::set_selected(char type, int32_t index)
	{
		m_type = type;
		m_index = index;

		switch (m_type)
		{
		case T_Reg:
			m_preview.loadFromFile(get_selected_name());
			break;

		case T_Dir:
			if (!ML_FileSystem.getDirContents(pathTo(get_selected_name()), m_preview.data()))
			{
				m_preview = get_selected_name();
			}
			break;

		default:
			m_preview = get_selected_name();
			break;
		}
	}

	char Browser::get_selected_type() const
	{
		return m_type;
	}

	String Browser::get_selected_name() const
	{
		const String * file;
		return ((file = getFile())
			? (*file)
			: String());
	}

	String Browser::get_selected_ext() const
	{
		switch (m_type)
		{
		case T_Reg	: return ML_FileSystem.getFileExt(get_selected_name());
		case T_Dir	: return String("Directory");
		case T_Lnk	: return String("Link");
		default		: return String();
		}
	}

	size_t Browser::get_selected_size() const
	{
		switch (m_type)
		{
		case T_Reg	: ML_FileSystem.getFileSize(get_selected_name());
		default		: return 0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}