#include <MemeEditor/Builder.hpp>
#include <MemeEditor/ResourceManager.hpp>
#include <MemeEditor/GUI.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	inline static void copy_range(Builder::SourceBuf & buf, size_t index, const String & str)
	{
		for (size_t i = 0; i < str.size(); i++)
		{
			buf[index + i] = str[i];
		}
	}
}

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
			"\n"
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
		ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
		if (beginDraw(p_open))
		{
			ImGui::BeginGroup();
			{
				// Information
				ImGui::BeginChild("Tabs");
				{
					// Tabs
					if (ImGui::BeginTabBar("##Tabs"))
					{
						draw_shader_tab("Vertex", m_src["Vertex"]);
						draw_shader_tab("Geometry", m_src["Geometry"]);
						draw_shader_tab("Fragement", m_src["Fragement"]);

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

			if (ImGui::BeginTabBar((label + "##Tabs").c_str()))
			{
				if (ImGui::BeginTabItem(("Uniforms##" + label).c_str()))
				{
					edit_uniform_list(label);

					ImGui::SameLine();

					if (ImGui::BeginChild(
						("Builder##" + label + "##Tabs").c_str(),
						(ImVec2(-1.0f, -1.0f)),
						(true),
						(ImGuiWindowFlags_AlwaysHorizontalScrollbar)))
					{
						edit_uniform_data(get_selected(label));

						ImGui::EndChild();
					}

					ImGui::EndTabItem();
				}

				draw_source_tab(("Source##" + label).c_str(), source);

				ImGui::EndTabBar();
			}
			ImGui::EndTabItem();
		}
	}

	void Builder::edit_uniform_data(Uniform * value)
	{
		if (!value) 
		{
			return ImGui::Text("Nothing Selected"); 
		}

		ImGui::PushID(value->name.c_str());
		{
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
		}
		ImGui::PopID();

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Builder::edit_uniform_list(const String & value)
	{
		ImGui::BeginChild("Uniform List", { 224, 0 }, true);
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
		}
		ImGui::EndChild();
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

	void Builder::draw_source_tab(CString label, SourceBuf & source)
	{
		if (ImGui::BeginTabItem(label))
		{
			ImGui::BeginChild(
				("Content"),
				(ImVec2(-1.0f, -1.0f)),
				(true),
				(ImGuiWindowFlags_AlwaysHorizontalScrollbar));
			{
				if (ImGui::InputTextMultiline(
					("Source"),
					(source),
					(IM_ARRAYSIZE(source)),
					(ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16)),
					(ImGuiInputTextFlags_AllowTabInput),
					(NULL),
					(NULL)))
				{

				}

				if (ImGui::Button("Compile")) {}
			}
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}