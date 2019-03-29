#include <MemeEditor/Builder.hpp>
#include <MemeEditor/ResourceManager.hpp>
#include <MemeEditor/GUI.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEditor/Terminal.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Builder::Builder()
		: GUI_Window("Builder")
		, m_selected(0)
	{
		strcpy(m_src["Vertex"],
			"#include <common/Vert.MVP.shader>\n"
			"#shader vertex\n"
			"\n"
			"void main()\n"
			"{\n"
			"\tgl_Position = ml_MVP_Position();\n"
			"}\n"
		);
		strcpy(m_src["Geometry"],
			"#include <common/Geom.Curve.shader>\n"
			"#shader geometry\n"
			"\n"
			"void main()\n"
			"{\n"
			"\t//...\n"
			"}\n"
		);
		strcpy(m_src["Fragement"],
			"#include <common/Frag.draw.shader>\n"
			"#shader fragment\n"
			"\n"
			"void main()\n"
			"{\n"
			"\tgl_Color = Frag.mainCol * texture(Frag.mainTex, In.Texcoord);\n"
			"}\n"
		);
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
					if (ImGui::BeginTabBar("Tab Bar"))
					{
						/* * * * * * * * * * * * * * * * * * * * */

						draw_shader_tab("Vertex", m_src["Vertex"]);
						draw_shader_tab("Geometry", m_src["Geometry"]);
						draw_shader_tab("Fragement", m_src["Fragement"]);

						/* * * * * * * * * * * * * * * * * * * * */

						if (ImGui::BeginTabItem("Tools"))
						{
							if (ImGui::Button("Compile"))
							{
								ML_Terminal.printf("[ LOG ] Compile Pressed");
							}

							ImGui::EndTabItem();
						}

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

	void Builder::draw_shader_tab(const String & label, SourceBuf & source)
	{
		if (ImGui::BeginTabItem(label.c_str()))
		{
			ImGui::Text(label.c_str());

			draw_uniform_list(label);

			ImGui::SameLine();

			draw_uniform_data(get_selected(label));

			ImGui::SameLine();

			draw_shader_source(("Source##" + label).c_str(), source);

			ImGui::EndTabItem();
		}
	}

	void Builder::draw_uniform_list(const String & value)
	{
		if (ImGui::BeginChild("Uniform List", { 224, 0 }, true))
		{
			// Uniform List
			/* * * * * * * * * * * * * * * * * * * * */
			for (size_t i = 0, imax = m_uni[value].size(); i < imax; i++)
			{
				if (Uniform * u = get_uniform(value, i))
				{
					if (ImGui::Selectable(
						(u->name.c_str()),
						(m_selected == i),
						(ImGuiSelectableFlags_AllowDoubleClick)))
					{
						m_selected = i;
					}
				}
			}
			ImGui::Separator();

			// Uniform List Buttons
			/* * * * * * * * * * * * * * * * * * * * */
			draw_uniform_buttons(m_uni[value]);

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
				(Document::NameSize),
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
				ImGui::DragInt("Value", &temp);
			}
			break;
			case Uniform::Float:
			{
				static float temp = 0.f;
				temp = value->get_value<float>(0.f);
				ImGui::DragFloat("Value", &temp, 0.1f);
			}
			break;
			case Uniform::Vec2:
			{
				static vec2f temp = 0.f;
				temp = value->get_value<vec2f>(0.f);
				GUI::EditVec2f("Value", temp);
			}
			break;
			case Uniform::Vec3:
			{
				static vec3f temp = 0.f;
				temp = value->get_value<vec3f>(0.f);
				GUI::EditVec3f("Value", temp);
			}
			break;
			case Uniform::Vec4:
			{
				static vec4f temp = 0.f;
				temp = value->get_value<vec4f>(0.f);
				GUI::EditVec4f("Value", temp);
			}
			break;
			case Uniform::Mat3:
			{
				static mat3f temp = 0.f;
				temp = value->get_value<mat3f>(0.f);
				GUI::EditMat3f("Value", temp);
			}
			break;
			case Uniform::Mat4:
			{
				static mat4f temp = 0.f;
				temp = value->get_value<mat4f>(0.f);
				GUI::EditMat4f("Value", temp);
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

				}
			}
			break;
			}

			/* * * * * * * * * * * * * * * * * * * * */
		
			ImGui::PopID(); 
			
			ImGui::EndChild();
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Builder::draw_uniform_buttons(List<Uniform> & value)
	{
		ImGui::BeginGroup();
		{
			if (ImGui::Button("Up"))
			{
				if (!value.empty() && m_selected > 0)
				{
					std::swap(value[m_selected], value[m_selected - 1]);
					m_selected--;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Dn"))
			{
				if (m_selected + 1 < value.size())
				{
					std::swap(value[m_selected], value[m_selected + 1]);
					m_selected++;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("New"))
			{
				value.push_back(Uniform("New.Uniform"));
				m_selected = value.size() - 1;
			}
			ImGui::SameLine();
			if (ImGui::Button("Ins"))
			{
				if (value.size() > 0)
				{
					value.insert(value.begin() + m_selected, Uniform("New.Uniform"));
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Dup"))
			{
				if (value.size() > 0)
				{
					value.push_back(Uniform(value[m_selected]));
					m_selected = value.size() - 1;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Del"))
			{
				if (value.size() > 0)
				{
					value.erase(value.begin() + m_selected);
					m_selected = (m_selected > 0 ? m_selected - 1 : value.size() - 1);
				}
			}
		}
		ImGui::EndGroup();
	}

	void Builder::draw_shader_source(CString label, SourceBuf & source)
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
				(IM_ARRAYSIZE(source)),
				{ -1.f, -1.f },
				(ImGuiInputTextFlags_AllowTabInput),
				(NULL),
				(NULL)))
			{
			}
			
			ImGui::EndChild();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}