#include <MemeEditor/Browser.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeCore/EventSystem.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_ml.hpp>

namespace ml
{
	Browser::Browser()
		: m_path	()
		, m_dir		()
		, m_type	('/')
		, m_index	(0)
		, m_preview	()
		, m_isDouble(false)
	{
		ML_EventSystem.addListener(CoreEvent::EV_FileSystem, this);

		update(ML_FileSystem.getWorkingDir());
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
				update(ML_FileSystem.getWorkingDir());
			}
			break;
		}
	}

	void Browser::update(const String & path)
	{
		if (!ML_FileSystem.getDirContents(path, m_dir))
		{
			Debug::logError("Failed Reading Directory: {0}", m_path);
		}
	}

	void Browser::draw(bool * p_open)
	{
		ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Browser", p_open, ImGuiWindowFlags_MenuBar))
		{
			ImGui::End();
			return;
		}
		else
		{
			// menu bar
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Close")) { (*p_open) = false; }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::Text("%s", ML_FileSystem.getWorkingDir().c_str());
			
			draw_file_list();
			ImGui::SameLine();
			draw_file_data();

			// Handle Double Clicks
			if (m_isDouble)
			{
				m_isDouble = false;
				switch (m_type)
				{
				case '/': 
				if (!ML_FileSystem.setWorkingDir(selectedFile()))
				{
					Debug::logError("Failed Opening Directory {0}", selectedFile());
				}
				break;
				}
			}
			
		}
		ImGui::End();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Browser::draw_file_list()
	{
		ImGui::BeginChild("Directory View", { 256, 0 }, true);
		{
			m_isDouble = false;

			for (const Pair<char, List<String>> & pair : m_dir)
			{
				for (size_t i = 0; i < pair.second.size(); i++)
				{
					if (ImGui::Selectable(
						(pair.second.at(i).c_str()),
						(isSelected(pair.first, i)),
						(ImGuiSelectableFlags_AllowDoubleClick)))
					{
						setSelected(pair.first, i, ImGui::IsMouseDoubleClicked(0));
					}
				}
			}
		}
		ImGui::EndChild();
	}

	void Browser::draw_file_data()
	{
		ImGui::BeginGroup();
		{
			ImGui::BeginChild("Content View", { 0, -ImGui::GetFrameHeightWithSpacing() });
			{
				ImGui::Separator();

				if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
				{
					if (ImGui::BeginTabItem("Preview"))
					{
						switch (m_type)
						{
						case ' ':
							ImGui::TextWrapped("%s", (CString)m_preview);
							break;
						default: 
							ImGui::TextWrapped("%s", selectedFile().c_str());
							break;
						}
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Details"))
					{
						ImGui::Text("Name: %s", selectedFile().c_str());
						ImGui::Text("Type: %s", selectedType().c_str());
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
			}
			ImGui::EndChild();

			if (ImGui::Button("Revert"))
			{
			}
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{
			}
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Browser::setSelected(char type, size_t index, bool isDouble)
	{
		m_type = type;
		m_index = index;
		m_preview.loadFromFile(selectedFile());
		
		if (isDouble)
		{
			m_isDouble = true;
		}
	}

	String Browser::selectedFile() const
	{
		Directory::const_iterator it;
		if ((it = m_dir.find(m_type)) != m_dir.end())
		{
			if (m_index < it->second.size())
			{
				return it->second.at(m_index);
			}
		}
		return String();
	}

	String Browser::selectedType() const
	{
		switch (m_type)
		{
		case ' ': return ML_FileSystem.getFileExtension(selectedFile());
		case '/': return String("Directory");
		default	: return String("?");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}