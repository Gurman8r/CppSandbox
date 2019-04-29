#include <MemeEditor/Builder.hpp>
#include <MemeEngine/Resources.hpp>
#include <MemeEditor/GUI.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeGraphics/ShaderAPI.hpp>
#include <MemeGraphics/Uni.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Builder::BuilderData::BuilderData()
		: BuilderData(String())
	{
	}

	Builder::BuilderData::BuilderData(const String & name)
		: BuilderData(name, String())
	{
	}

	Builder::BuilderData::BuilderData(const String & name, const String & src)
		: BuilderData(name, src, UniformList())
	{
	}

	Builder::BuilderData::BuilderData(const String & name, const String & src, const UniformList & uni)
		: name(name)
		, uni(uni)
	{
		strcpy(this->src, src.c_str());
	}

	Builder::BuilderData::BuilderData(const BuilderData & copy)
		: BuilderData(copy.name, copy.src, copy.uni)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Builder::Builder()
		: GUI_Window("Builder")
		, m_selected(0)
		, m_shader(ML_Res.shaders.load(ML_TEST_SHADER))
	{
		set_data(BuilderData("Vertex",
			"#include <common/Vert.MVP.shader>\n"
			"#shader vertex\n"
			"void main()\n"
			"{\n"
			"\tgl_Position = ml_MVP_Position();\n"
			"}\n",
			{
				Uniform{ ML_VERT_MODEL,	uni_base::Mat4, 0 },
				Uniform{ ML_VERT_VIEW,	uni_base::Mat4, 0 },
				Uniform{ ML_VERT_PROJ,	uni_base::Mat4, 0 },
			}
		));
		
		set_data(BuilderData("Fragment",
			"#include <common/Frag.Draw.shader>\n"
			"#shader fragment\n"
			"void main()\n"
			"{\n"
			"\tgl_Color = Frag.mainCol * texture(Frag.mainTex, In.Texcoord);\n"
			"}\n",
			{
				Uniform{ ML_FRAG_MAIN_COL, uni_base::Col4, 0 },
				Uniform{ ML_FRAG_MAIN_TEX, uni_base::Tex, 0 },
			}
		));
		
		set_data(BuilderData("Geometry", "", {}));
	}

	Builder::~Builder()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Builder::onEvent(const IEvent * value)
	{
	}

	bool Builder::drawGui(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::BeginGroup();
			{
				ImGui::BeginChild("Tabs");
				{
					if (ImGui::BeginTabBar("Main Tab Bar"))
					{
						/* * * * * * * * * * * * * * * * * * * * */

						draw_shader_tab("Vertex");
						draw_shader_tab("Geometry");
						draw_shader_tab("Fragment");

						/* * * * * * * * * * * * * * * * * * * * */

						draw_tools();

						/* * * * * * * * * * * * * * * * * * * * */

						ImGui::EndTabBar();
					}
				}
				ImGui::EndChild();
			}
			ImGui::EndGroup();
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Builder::draw_shader_tab(const String & value)
	{
		if (ImGui::BeginTabItem(value.c_str()))
		{
			// Name
			ImGui::Text(value.c_str());

			// List
			draw_uniform_list(get_list(value));

			ImGui::SameLine();

			// Data
			//draw_uniform_data(get_selected_uniform(value));

			ImGui::SameLine();

			// Source
			draw_shader_text(get_source(value));

			ImGui::EndTabItem();
		}
	}

	void Builder::draw_uniform_list(UniformList * value)
	{
		if (ImGui::BeginChild("Uniform List", { 224, 0 }, true))
		{
			if (!value)
			{
				ImGui::Text("Nothing Selected");
				return ImGui::EndChild();
			}

			// List
			/* * * * * * * * * * * * * * * * * * * * */
			for (size_t i = 0, imax = value->size(); i < imax; i++)
			{
				if (Uniform * u = &(*value)[i])
				{
					if (ImGui::Selectable(
						(u->name.c_str()),
						(m_selected == i),
						(ImGuiSelectableFlags_AllowDoubleClick)))
					{
						m_selected = i;
					
						if (ImGui::IsMouseDoubleClicked(0))
						{
						}
					}
				}
			}
			ImGui::Separator();

			// Buttons
			/* * * * * * * * * * * * * * * * * * * * */
			ImGui::BeginGroup();
			{
				if (ImGui::Button("Up"))
				{
					if (!value->empty() && m_selected > 0)
					{
						std::swap(value[m_selected], value[m_selected - 1]);
						m_selected--;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Dn"))
				{
					if (m_selected + 1 < value->size())
					{
						std::swap(value[m_selected], value[m_selected + 1]);
						m_selected++;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("New"))
				{
					value->push_back(Uniform { "New.Uniform", 0, 0 });
					m_selected = value->size() - 1;
				}
				ImGui::SameLine();
				if (ImGui::Button("Ins"))
				{
					if (value->size() > 0)
					{
						value->insert(value->begin() + m_selected, Uniform { "New.Uniform", 0, 0 });
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Dup"))
				{
					if (value->size() > 0)
					{
						value->push_back(Uniform((*value)[m_selected]));
						m_selected = value->size() - 1;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Del"))
				{
					if (value->size() > 0)
					{
						value->erase(value->begin() + m_selected);
						m_selected = (m_selected > 0 ? m_selected - 1 : value->size() - 1);
					}
				}
			}
			ImGui::EndGroup();

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::EndChild();
		}
	}

	void Builder::draw_shader_text(char * source)
	{
		if (ImGui::BeginChild(
			("Shader Source"),
			{ 0, 0 },
			(true),
			(ImGuiWindowFlags_AlwaysHorizontalScrollbar)))
		{
			if (ImGui::InputTextMultiline(
				("Source Input"),
				(source),
				(sizeof(SourceBuffer)),
				{ -1.f, -1.f },
				(ImGuiInputTextFlags_AllowTabInput),
				(NULL),
				(NULL)))
			{
				// changed
			}
			ImGui::EndChild();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Builder::draw_tools()
	{
		if (ImGui::BeginTabItem("Tools"))
		{
			if (ImGui::BeginTabBar("Tool Tabs"))
			{
				// Shader Source
				/* * * * * * * * * * * * * * * * * * * * */
				SStream src;
				src << "/* * * * * * * * * * * * * * * * * * * * */\n\n"
					<< get_source("Vertex") << endl
					<< "/* * * * * * * * * * * * * * * * * * * * */\n\n"
					<< get_source("Geometry") << endl
					<< "/* * * * * * * * * * * * * * * * * * * * */\n\n"
					<< get_source("Fragment") << endl
					<< "/* * * * * * * * * * * * * * * * * * * * */\n\n";

				// Actions
				/* * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Actions"))
				{
					if (ImGui::Button("Compile")) 
					{
						if (m_shader && m_shader->loadFromMemory(src.str()))
						{
							Debug::log("Compiled Shader");
						}
						else
						{
							Debug::logError("Failed Compiling Shader");
						}
					}

					ImGui::EndTabItem();
				}


				// Output
				/* * * * * * * * * * * * * * * * * * * * */
				if (ImGui::BeginTabItem("Output"))
				{
					ImGui::InputTextMultiline(
						("##Output"),
						(&src.str()[0]),
						(src.str().size()),
						{ -1.f, -1.f },
						(ImGuiInputTextFlags_ReadOnly));

					ImGui::EndTabItem();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::EndTabBar();
			}

			ImGui::EndTabItem();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}