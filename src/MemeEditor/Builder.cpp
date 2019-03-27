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
		copy_range(m_s["Vertex"], 0, 
			"#shader vertex\n"
			"#version 410 core\n"
		);
		copy_range(m_s["Geometry"], 0, 
			"#shader geometry\n"
			"#version 410 core\n"
		);
		copy_range(m_s["Fragement"], 0, 
			"#shader fragment\n"
			"#version 410 core\n"
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
						draw_shader_tab("Vertex", m_s["Vertex"]);
						draw_shader_tab("Geometry", m_s["Geometry"]);
						draw_shader_tab("Fragement", m_s["Fragement"]);

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
					draw_uniform_list(label);

					ImGui::SameLine();

					if (ImGui::BeginChild(
						("ResourceLoader##" + label + "##Tabs").c_str(),
						(ImVec2(-1.0f, -1.0f)),
						(true),
						(ImGuiWindowFlags_AlwaysHorizontalScrollbar)))
					{
						draw_uniform_data(get_selected(label));

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

	void Builder::draw_uniform_data(Uniform * value)
	{
		if (!value)
		{
			ImGui::Text("Nothing Selected");
			return;
		}

		static CString u_types[] = {
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

		ImGui::PushID(value->name.c_str());
		{
			// Name
			/* * * * * * * * * * * * * * * * * * * * */
			static const ImGuiInputTextFlags flags = 
				ImGuiInputTextFlags_EnterReturnsTrue |
				ImGuiInputTextFlags_CallbackCompletion |
				ImGuiInputTextFlags_CallbackHistory;
			auto textEditCallback = [](ImGuiInputTextCallbackData * data)
			{
				if (Builder * b = static_cast<Builder *>(data->UserData))
				{
					return 1;
				}
				return 0;
			};
			bool reclaim_focus = false;
			if (ImGui::InputText(
				"Name",
				m_inputBuf,
				IM_ARRAYSIZE(m_inputBuf),
				flags,
				textEditCallback,
				(void *)(this)
				))
			{
				reclaim_focus = true;
			}
			ImGui::SetItemDefaultFocus();
			if (reclaim_focus)
			{
				ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
			}

			// Type
			/* * * * * * * * * * * * * * * * * * * * */
			ImGui::Combo("Type", &value->type, u_types, IM_ARRAYSIZE(u_types));

			// Data
			/* * * * * * * * * * * * * * * * * * * * */
			switch (value->type)
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
				GUI::EditVec2f("Value", temp);
			}
			break;
			case Uniform::Vec3:
			{
				static vec3f temp;
				GUI::EditVec3f("Value", temp);
			}
			break;
			case Uniform::Vec4:
			{
				static vec4f temp;
				GUI::EditVec4f("Value", temp);
			}
			break;
			case Uniform::Mat3:
			{
				static mat3f temp;
				GUI::EditMat3f("Value", temp);
			}
			break;
			case Uniform::Mat4:
			{
				static mat4f temp;
				GUI::EditMat4f("Value", temp);
			}
			break;
			case Uniform::Tex2D:
			{
				auto vector_getter = [](void* vec, int idx, const char** out_text)
				{
					auto& vector = *static_cast<List<String>*>(vec);
					if (idx < 0 || idx >= static_cast<int32_t>(vector.size())) 
					{ 
						return false; 
					}
					*out_text = vector.at(idx).c_str();
					return true;
				};

				const ResourceManager::TextureMap & textures = ML_Res.textures;
				List<String> names;
				for (auto pair : textures)
				{
					names.push_back(pair.first);
				}

				static int32_t index = 0;

				ImGui::Combo(
					"Texture", 
					&index,
					vector_getter, 
					static_cast<void *>(&names), 
					(int32_t)names.size());
			}
			break;
			}
		}
		ImGui::PopID();
	}

	void Builder::draw_uniform_list(const String & value)
	{
		ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_Text);

		ImGui::BeginChild("List View", { 224, 0 }, true);
		{
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			for (size_t i = 0, imax = m_u[value].size(); i < imax; i++)
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
			ImGui::PopStyleColor();
			ImGui::Separator();

			// Buttons
			draw_uniform_list_buttons(m_u[value]);
		}
		ImGui::EndChild();
	}

	void Builder::draw_uniform_list_buttons(List<Uniform> & value)
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
				value.push_back(Uniform("new_uniform"));
				m_selected = value.size() - 1;
			}
			ImGui::SameLine();
			if (ImGui::Button("Ins"))
			{
				if (value.size() > 0)
				{
					value.insert(value.begin() + m_selected, Uniform("new_uniform"));
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
			// Vertex
			if (ImGui::BeginTabItem(label))
			{
				ImGui::BeginChild(
					("Content"),
					(ImVec2(-1.0f, -1.0f)),
					(true),
					(ImGuiWindowFlags_AlwaysHorizontalScrollbar));
				{
					ImGui::InputTextMultiline(
						"Source", 
						source, 
						IM_ARRAYSIZE(source),
						ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
						ImGuiInputTextFlags_AllowTabInput,
						NULL, 
						NULL);

					if (ImGui::Button("Compile")) {}
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}