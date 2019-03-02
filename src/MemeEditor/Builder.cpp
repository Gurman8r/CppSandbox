#include <MemeEditor/Builder.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_ml.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Builder::Builder()
	{
	}

	Builder::~Builder()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Builder::draw(bool * p_open)
	{
		ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Builder", (m_open = p_open), ImGuiWindowFlags_MenuBar))
		{
			ImGui::End();
			return;
		}
		else
		{
			//editSource(m_source);
			//editUniforms(m_uniforms);

			draw_menu();

			draw_list();

			ImGui::SameLine();

			draw_preview();
		}
		ImGui::End();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Builder::draw_menu()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void Builder::draw_list()
	{
		ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_Text);

		ImGui::BeginChild("Directory View", { 256, 0 }, true);
		{
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			for (size_t i = 0, imax = m_uniforms.size(); i < imax; i++)
			{
				if (Uniform * u = get_uniform(i))
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
			ImGui::PopStyleColor();
			ImGui::Separator();

			// Buttons
			ImGui::BeginGroup();
			{
				if (ImGui::Button("Up"))
				{
					if (!m_uniforms.empty() && m_selected > 0)
					{
						std::swap(m_uniforms[m_selected], m_uniforms[m_selected - 1]);
						m_selected--;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Dn"))
				{
					if (m_selected + 1 < m_uniforms.size())
					{
						std::swap(m_uniforms[m_selected], m_uniforms[m_selected + 1]);
						m_selected++;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("New"))
				{
					m_uniforms.push_back(Uniform("new_uniform"));
					m_selected = m_uniforms.size() - 1;
				}
				ImGui::SameLine();
				if (ImGui::Button("Ins"))
				{
					if (m_uniforms.size() > 0)
					{
						m_uniforms.insert(m_uniforms.begin() + m_selected, Uniform("new_uniform"));
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Dup"))
				{
					if (m_uniforms.size() > 0)
					{
						m_uniforms.push_back(Uniform(m_uniforms[m_selected]));
						m_selected = m_uniforms.size() - 1;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Del"))
				{
					if (m_uniforms.size() > 0)
					{
						m_uniforms.erase(m_uniforms.begin() + m_selected);
						m_selected = (m_selected > 0 ? m_selected - 1 : m_uniforms.size() - 1);
					}
				}
			}
			ImGui::EndGroup();
		}
		ImGui::EndChild();
	}

	void Builder::draw_preview()
	{
		ImGui::BeginGroup();
		{
			// Information
			ImGui::BeginChild("Tabs");
			{
				// Tabs
				if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
				{
					// Uniform
					if (ImGui::BeginTabItem("Uniform"))
					{
						ImGui::BeginChild(
							("Content"),
							(ImVec2(-1.0f, -1.0f)),
							(true),
							(ImGuiWindowFlags_AlwaysHorizontalScrollbar));
						{
							if (Uniform * u = get_selected())
							{
								draw_uniform(*u);
							}
							else
							{
								ImGui::Text("Nothing Selected");
							}
						}
						ImGui::EndChild();
						ImGui::EndTabItem();
					}
					// Source
					if (ImGui::BeginTabItem("Source"))
					{
						ImGui::BeginChild(
							("Content"),
							(ImVec2(-1.0f, -1.0f)),
							(true),
							(ImGuiWindowFlags_AlwaysHorizontalScrollbar));
						{
							ImGui::TextUnformatted(
								&m_source[0],
								&m_source[m_source.size()]);

							if (ImGui::Button("Compile")) {}
						}
						ImGui::EndChild();
						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}
			}
			ImGui::EndChild();
		}
		ImGui::EndGroup();
	}

	void Builder::draw_uniform(Uniform & uniform)
	{
		static CString u_types[] = {
			"None",
			"Int",
			"Float",
			"Vec2",
			"Vec3",
			"Vec4",
			"Mat3",
			"Mat4",
			"Tex",
		};

		ImGui::PushID(uniform.name.c_str());
		ImGui::InputText("Name", &uniform.name[0], 32);
		ImGui::Combo("Type", &uniform.type, u_types, IM_ARRAYSIZE(u_types));
		switch (uniform.type)
		{
		case Uniform::Int:
		{
			static int32_t temp;
			ImGui::DragInt("Value", &temp);
		}
		break;
		case Uniform::Float:
		{
			static float temp;
			ImGui::DragFloat("Value", &temp, 0.1f);
		}
		break;
		case Uniform::Vec2:
		{
			static vec2f temp;
			ML_Editor.InputVec2f("Value", temp);
		}
		break;
		case Uniform::Vec3:
		{
			static vec3f temp;
			ML_Editor.InputVec3f("Value", temp);
		}
		break;
		case Uniform::Vec4:
		{
			static vec4f temp;
			ML_Editor.InputVec4f("Value", temp);
		}
		break;
		case Uniform::Mat3:
		{
			static mat3f temp;
			ML_Editor.InputMat3f("Value", temp);
		}
		break;
		case Uniform::Mat4:
		{
			static mat4f temp;
			ML_Editor.InputMat4f("Value", temp);
		}
		break;
		case Uniform::Tex:
			break;
		}
		ImGui::PopID();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}