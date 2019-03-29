#include <MemeEditor/Builder.hpp>
#include <MemeEditor/ResourceManager.hpp>
#include <MemeEditor/GUI.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEditor/Terminal.hpp>
#include <MemeGraphics/ShaderParser.hpp>
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
		, m_shader(ML_Res.shaders.get(ML_TEST_SHADER))
	{
		set_data(BuilderData("Vertex",
			"#include <common/Vert.MVP.shader>\n"
			"#shader vertex\n"
			"\n"
			"void main()\n"
			"{\n"
			"\tgl_Position = ml_MVP_Position();\n"
			"}\n",
			{
				Uniform("Vert.model", Uniform::Mat4),
				Uniform("Vert.view", Uniform::Mat4),
				Uniform("Vert.proj", Uniform::Mat4),
			}
		));

		set_data(BuilderData("Geometry",
			"#include <common/Geom.Curve.shader>\n"
			"#shader geometry\n"
			"\n"
			"struct Curve_Uniforms\n"
			"{\n"
			"\tint		mode;\n"
			"\tfloat	delta;\n"
			"\tfloat	size;\n"
			"\tint		samples;\n"
			"};\n"
			"uniform Curve_Uniforms Curve;\n"
			"\n"
			"void main()\n"
			"{\n"
			"\tfloat size = Curve.size;\n"
			"\tvec4 testP0 = vec4(-size, +size, 0.0, 1.0);\n"
			"\tvec4 testP1 = vec4(+size, +size, 0.0, 1.0);\n"
			"\tvec4 testP2 = vec4(+size, -size, 0.0, 1.0);\n"
			"\tvec4 testP3 = vec4(-size, -size, 0.0, 1.0);\n"
			"\tdrawLineFull(testP0, testP1);\n"
			"\tdrawLineFull(testP1, testP2);\n"
			"\tdrawLineFull(testP2, testP3);\n"
			"\tdrawLineFull(testP3, testP0);\n"
			"}\n",
			{
				Uniform("Curve.mode",	Uniform::Int),
				Uniform("Curve.delta",	Uniform::Float),
				Uniform("Curve.size",	Uniform::Float),
				Uniform("Curve.samples",Uniform::Int),
			}
		));

		set_data(BuilderData("Fragment",
			"#include <common/Frag.Draw.shader>\n"
			"#shader fragment\n"
			"\n"
			"void main()\n"
			"{\n"
			"\tgl_Color = Frag.mainCol * texture(Frag.mainTex, In.Texcoord);\n"
			"}\n",
			{
				Uniform("Frag.mainCol", Uniform::Vec4),
				Uniform("Frag.mainTex", Uniform::Tex2D),
			}
		));
	}

	Builder::~Builder()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Builder::onEvent(const IEvent * value)
	{
	}

	bool Builder::draw(bool * p_open)
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
			draw_uniform_data(get_selected_uniform(value));

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
					value->push_back(Uniform("New.Uniform"));
					m_selected = value->size() - 1;
				}
				ImGui::SameLine();
				if (ImGui::Button("Ins"))
				{
					if (value->size() > 0)
					{
						value->insert(value->begin() + m_selected, Uniform("New.Uniform"));
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

	void Builder::draw_uniform_data(Uniform * value)
	{
		if (ImGui::BeginChild(
			("Uniform Data"),
			{ 300, 0 },
			(true),
			(ImGuiWindowFlags_None)))
		{
			if (!value)
			{
				ImGui::Text("Nothing Selected");
				return ImGui::EndChild();
			}

			ImGui::PushID(value->name.c_str());

			// Name
			/* * * * * * * * * * * * * * * * * * * * */
			char name[64];
			strcpy(name, value->name.data());
			if (ImGui::InputText(
				("Name"),
				(name),
				(sizeof(name)),
				(ImGuiInputTextFlags_EnterReturnsTrue),
				(NULL),
				(NULL)))
			{
				value->name = String(name);
			}

			// Type
			/* * * * * * * * * * * * * * * * * * * * */
			static CString uni_types[] = {
				"None",
				"Int",
				"Float",
				"Vec2",
				"Vec3",
				"Vec4",
				"Mat3",
				"Mat4",
				"Tex2D",
			};
			if (ImGui::Combo(
				("Type"),
				(&value->type),
				(uni_types),
				(IM_ARRAYSIZE(uni_types))))
			{
			}

			// Data
			/* * * * * * * * * * * * * * * * * * * * */
			switch (value->type)
			{
			case Uniform::Int:
			{
				static int32_t temp = 0;
				temp = value->get_value<int32_t>(0);
				if (ImGui::DragInt("Value", &temp))
				{
					// changed
				}
			}
			break;
			case Uniform::Float:
			{
				static float temp = 0.f;
				temp = value->get_value<float>(0.f);
				if (ImGui::DragFloat("Value", &temp, 0.1f))
				{
					// changed
				}
			}
			break;
			case Uniform::Vec2:
			{
				static vec2f temp = 0.f;
				temp = value->get_value<vec2f>(0.f);
				if (GUI::EditVec2f("Value", temp))
				{
					// changed
				}
			}
			break;
			case Uniform::Vec3:
			{
				static vec3f temp = 0.f;
				temp = value->get_value<vec3f>(0.f);
				if (GUI::EditVec3f("Value", temp))
				{
					// changed
				}
			}
			break;
			case Uniform::Vec4:
			{
				static vec4f temp = 0.f;
				temp = value->get_value<vec4f>(0.f);
				if (GUI::EditVec4f("Value", temp))
				{
					// changed
				}
			}
			break;
			case Uniform::Mat3:
			{
				static mat3f temp = 0.f;
				temp = value->get_value<mat3f>(0.f);
				if (GUI::EditMat3f("Value", temp))
				{
					// changed
				}
			}
			break;
			case Uniform::Mat4:
			{
				static mat4f temp = 0.f;
				temp = value->get_value<mat4f>(0.f);
				if (GUI::EditMat4f("Value", temp))
				{
					// changed
				}
			}
			break;
			case Uniform::Tex2D:
			{
				auto vector_getter = [](void * vec, int idx, CString * out)
				{
					auto & vector = (*static_cast<List<String>*>(vec));
					if (idx >= 0 && idx < static_cast<int32_t>(vector.size()))
					{
						(*out) = vector.at(idx).c_str();
						return true;
					}
					return false;
				};

				List<String> names;
				for (auto pair : ML_Res.textures)
				{
					names.push_back(pair.first);
				}

				static int32_t index = 0;

				if (ImGui::Combo(
					"Texture",
					&index,
					vector_getter,
					static_cast<void *>(&names),
					(int32_t)names.size()))
				{
					// changed
				}
			}
			break;
			}

			ImGui::PopID();
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
						IO::capture_cout([&](SStream & ss)
						{
							if (m_shader->loadFromMemory(src.str()))
							{
								Debug::log("Compiled Shader");
							}
							else
							{
								Debug::logError("Failed Compiling Shader");
							}

							ML_Terminal.print(ss);
						});
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