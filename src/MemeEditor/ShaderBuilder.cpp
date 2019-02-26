#include <MemeEditor/ShaderBuilder.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_ml.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ShaderBuilder::ShaderBuilder()
	{
	}

	ShaderBuilder::~ShaderBuilder()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void ShaderBuilder::draw(CString title, bool * p_open)
	{
		if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}
		else
		{
			editSource(m_source);

			editUniforms(m_uniforms);

			ImGui::End();
		}
	}

	void ShaderBuilder::editSource(String & source)
	{
		// Shader Source
		ImGui::BeginGroup();
		{
			ImGui::Text("Source:");

			ImGui::InputTextMultiline(
				"##Source",
				&source[0],
				source.capacity(),
				{ 512, ImGui::GetTextLineHeight() * 16 },
				0);

			if (ImGui::Button("Compile")) {}
		}
		ImGui::EndGroup();
	}

	void ShaderBuilder::editUniforms(List<Uniform> & values)
	{
		// Uniform Editor
		ImGui::PushItemWidth(256);
		{
			editUniformList(values);

			// Edit Uniforms
			ImGui::SameLine();
			ImGui::BeginGroup();
			{
				ImGui::Text("Edit:");

				if (Uniform * u = (values.empty() ? NULL : &values[m_selected]))
				{
					editUniformValue(*u);
				}
				else
				{
					ImGui::Text("Nothing Selected");
				}
			}
			ImGui::EndGroup();
		}
		ImGui::PopItemWidth();
	}

	void ShaderBuilder::editUniformList(List<Uniform> & values)
	{
		// List Uniforms
		ImGui::SameLine();
		ImGui::BeginGroup();
		{
			const int32_t count = (int32_t)values.size();

			ImGui::Text("Uniforms:");

			// List
			if (ImGui::ListBoxHeader("##Uniforms", count))
			{
				for (size_t i = 0; i < count; i++)
				{
					const String name = (std::to_string(i) + " : " + values[i].name);
					if (ImGui::Selectable(name.c_str(), (i == m_selected)))
					{
						m_selected = i;
					}
				}
				ImGui::ListBoxFooter();
			}

			// Buttons
			ImGui::BeginGroup();
			{
				if (ImGui::Button("Up"))
				{
					if (m_selected > 0)
					{
						std::swap(values[m_selected], values[m_selected - 1]);
						m_selected--;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Dn"))
				{
					if (m_selected + 1 < count)
					{
						std::swap(values[m_selected], values[m_selected + 1]);
						m_selected++;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("New"))
				{
					values.push_back(Uniform("new_uniform"));
					m_selected = values.size() - 1;
				}
				ImGui::SameLine();
				if (ImGui::Button("Ins"))
				{
					if (count > 0)
					{
						values.insert(values.begin() + m_selected, Uniform("new_uniform"));
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Dup"))
				{
					if (count > 0)
					{
						values.push_back(Uniform(values[m_selected]));
						m_selected = values.size() - 1;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Del"))
				{
					if (count > 0)
					{
						values.erase(values.begin() + m_selected);
						m_selected = (m_selected > 0 ? m_selected - 1 : values.size() - 1);
					}
				}
			}
			ImGui::EndGroup();
		}
		ImGui::EndGroup();
	}

	void ShaderBuilder::editUniformValue(Uniform & uniform)
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