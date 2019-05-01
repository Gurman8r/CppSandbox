#include <MemeEditor/Builder.hpp>
#include <MemeEngine/Resources.hpp>
#include <MemeEditor/GUI.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeGraphics/ShaderAPI.hpp>
#include <MemeGraphics/ShaderParser.hpp>
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
		m_files.push_back(new BuildFile("Vertex", ML_VERT_EXAMPLE));
		m_files.push_back(new BuildFile("Fragment", ML_FRAG_EXAMPLE));
		//m_files.push_back(new BuildFile("Geometry", ML_GEOM_EXAMPLE));
	}

	Builder::~Builder()
	{
		for (auto it : m_files)
		{
			delete it;
		}
		m_files.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Builder::onEvent(const IEvent * value)
	{
	}

	bool Builder::drawGui(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			// Menu Bar
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenuBar())
			{
				// New
				/* * * * * * * * * * * * * * * * * * * * */
				if (ImGui::MenuItem("New"))
				{
					ImGui::OpenPopup("New File");
				}
				if (ImGui::BeginPopupModal("New File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					static char name[32] = "New File";

					auto closePopup = [&]()
					{
						std::strcpy(name, "New File");
						ImGui::CloseCurrentPopup();
					};

					ImGui::InputText("Name", name, IM_ARRAYSIZE(name),
						ImGuiInputTextFlags_EnterReturnsTrue
					);

					if (ImGui::Button("Submit"))
					{
						auto addNewFile = [&]()
						{
							if (!String(name))
							{
								Debug::logError("Name cannot be empty");
								return;
							}

							for (auto file : m_files)
							{
								if (file->name == name)
								{
									Debug::logError("File with name \'{0}\' already exists", name);
									return;
								}
							}

							m_files.push_back(new BuildFile(name, String()));
						};
						addNewFile();
						closePopup();
					}
					ImGui::SameLine();
					if (ImGui::Button("Cancel"))
					{
						closePopup();
					}

					ImGui::EndPopup();
				}

				// Compile
				/* * * * * * * * * * * * * * * * * * * * */
				if (ImGui::MenuItem("Compile"))
				{
					struct BuilderParser
					{
						inline static String parseIncludes(
							const List<BuildFile *> & files, const String & src
						)
						{
							SStream out;
							SStream ss(src);
							String line;
							while (std::getline(ss, line))
							{
								if (line.find("#include") != String::npos)
								{
									String name;
									if (ShaderParser::parseWrapped(line, '\"', '\"', name))
									{
										bool found = false;
										for (auto f : files)
										{
											if (f->name == name)
											{
												out << parseIncludes(files, String(f->text));
												found = true;
												break;
											}
										}
										if (found)
										{
											break;
										}
									}
								}

								out << line << endl;
							}

							return out.str();
						}
					};


					SStream ss;
					ss	<< BuilderParser::parseIncludes(m_files, m_files[0]->text)
						<< BuilderParser::parseIncludes(m_files, m_files[1]->text);
					
					if (m_shader && m_shader->loadFromMemory(ss.str()))
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

			// File Tabs Bar
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			ImGui::BeginGroup();
			ImGui::BeginChild("Shader Content Tabs");
			if (ImGui::BeginTabBar("Shader Content Tabs", ImGuiTabBarFlags_Reorderable))
			{
				/* * * * * * * * * * * * * * * * * * * * */

				std::vector<BuildMap::iterator> toRemove;

				/* * * * * * * * * * * * * * * * * * * * */

				for (auto it = m_files.begin(); it != m_files.end(); it++)
				{
					BuildFile *	file = (*it);
				
					const size_t i = (it - m_files.begin());

					bool * t_open = i > 1 ? &file->open : NULL;

					// File Tab
					if (ImGui::BeginTabItem(
						String("[" + std::to_string(i) + "] " + file->name).c_str(),
						t_open,
						0//file->dirty ? ImGuiTabItemFlags_UnsavedDocument : 0
					))
					{	// Input Text Content Area
						if (ImGui::BeginChild(
							"InputTextContentArea",
							{ 0, 0 },
							true,
							ImGuiWindowFlags_AlwaysHorizontalScrollbar
						))
						{	/* * * * * * * * * * * * * * * * * * * * */

							if (ImGui::InputTextMultiline(
								String("##File" + file->name + "##Text").c_str(),
								file->text,
								BuildFile::MaxSize,
								{ -1.f, -1.f },
								ImGuiInputTextFlags_AllowTabInput
							))
							{
								file->dirty = true;
							}

							/* * * * * * * * * * * * * * * * * * * * */

							ImGui::EndChild();
						}
						ImGui::EndTabItem();
					}

					if (!file->open)
					{
						toRemove.push_back(it);
					}
				}

				/* * * * * * * * * * * * * * * * * * * * */

				for (auto it : toRemove)
				{
					delete (*it);
					m_files.erase(it);
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