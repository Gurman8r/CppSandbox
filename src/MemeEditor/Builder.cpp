#include <MemeEditor/Builder.hpp>
#include <MemeEngine/Resources.hpp>
#include <MemeEditor/GUI.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeGraphics/ShaderAPI.hpp>
#include <MemeGraphics/Material.hpp>
#include <MemeGraphics/Uni.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Builder::Builder()
		: GUI_Window("Builder")
		, m_shader(ML_Res.shaders.load(ML_TEST_SHADER))
	{
		createFile("Vertex",	ML_VERT_EXAMPLE);
		createFile("Fragment",	ML_FRAG_EXAMPLE);
		createFile("Geometry",	ML_GEOM_EXAMPLE);
	}

	Builder::~Builder()
	{
		for (auto & pair : m_data)
		{
			delete pair.second;
		}
		m_data.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Builder::onEvent(const IEvent * value)
	{
	}

	bool Builder::drawGui(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			// Store Full Source
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			SStream source;
			for (const auto & pair : m_data)
			{
				source << pair.second->data << endl << endl;
			}

			// Menu Bar
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::MenuItem("Compile"))
				{
					if (m_shader && m_shader->loadFromMemory(source.str()))
					{
						Debug::log("Compiled Shader: {0}", ML_TEST_SHADER);
					}
					else
					{
						Debug::logError("Failed Compiling Shader");
					}
				}

				ImGui::EndMenuBar();
			}

			// Tab Bar
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			ImGui::BeginGroup();
			ImGui::BeginChild("Tabs");
			if (ImGui::BeginTabBar("Main Tab Bar", ImGuiTabBarFlags_Reorderable))
			{
				/* * * * * * * * * * * * * * * * * * * * */
				
				for (auto & pair : m_data)
				{
					BuildFile *	file = pair.second;

					if (ImGui::BeginTabItem(file->name.c_str()))
					{
						if (ImGui::BeginChild(
							"ShaderSourceInput", 
							{ 0, 0 }, 
							true,
							ImGuiWindowFlags_AlwaysHorizontalScrollbar
						))
						{
							if (ImGui::InputTextMultiline(
								"##ShaderSourceInput",
								file->data,
								BuildFile::MaxSize,
								{ -1.f, -1.f },
								ImGuiInputTextFlags_AllowTabInput
							))
							{
								// changed
							}
							ImGui::EndChild();
						}
						ImGui::EndTabItem();
					}
				}

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::EndTabBar();
			}
			ImGui::EndChild();
			ImGui::EndGroup();

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}