#include <MemeEditor/Builder.hpp>
#include <MemeEditor/ResourceManager.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_ml.hpp>

namespace ml
{
	inline static void copy_range(List<char> & buf, size_t index, const String & str)
	{
		if (buf.size() < str.size())
		{
			buf = List<char>(str.begin(), str.end());
		}
		else if (str.size() <= buf.size())
		{
			for (size_t i = 0; i < str.size(); i++)
			{
				buf[index + i] = str[i];
			}
		}
	}
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Builder::Builder()
		: m_vert(ML_MAX_SRC)
		, m_geom(ML_MAX_SRC)
		, m_frag(ML_MAX_SRC)
	{
		copy_range(m_vert, 0, 
			"#shader vertex\n"
			"#version 410 core\n"
		);
		copy_range(m_geom, 0, 
			"#shader geometry\n"
			"#version 410 core\n"
		);
		copy_range(m_frag, 0, 
			"#shader fragment\n"
			"#version 410 core\n"
		);
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
			draw_list_buttons();
		}
		ImGui::EndChild();
	}

	void Builder::draw_list_buttons()
	{
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
							draw_uniform(get_selected());
						}
						ImGui::EndChild();
						ImGui::EndTabItem();
					}
					// Source
					{
						draw_source("Vertex", m_vert);
						draw_source("Geometry", m_geom);
						draw_source("Fragment", m_frag);
					}

					ImGui::EndTabBar();
				}
			}
			ImGui::EndChild();
		}
		ImGui::EndGroup();
	}

	void Builder::draw_uniform(Uniform * u)
	{
		if (!u)
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
			"Tex",
		};

		ImGui::PushID(u->name.c_str());
		{
			static const ImGuiInputTextFlags flags = 
				ImGuiInputTextFlags_EnterReturnsTrue |
				ImGuiInputTextFlags_CallbackCompletion |
				ImGuiInputTextFlags_CallbackHistory;

			//char * b = std::remove_cv_t<char *>(u->name.data());

			auto textEditCallback = [](ImGuiInputTextCallbackData * data)
			{
				if (Builder * b = static_cast<Builder *>(data->UserData))
				{
					b->textEditCallback();
				}
				return 0;
			};

			bool reclaim_focus = false;
			if (ImGui::InputText(
				"Name",
				m_buf,
				IM_ARRAYSIZE(m_buf),
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

			//u->name = String(buf);
			//ImGui::InputText("Name", &u->name[0], 32);
			
			ImGui::Combo("Type", &u->type, u_types, IM_ARRAYSIZE(u_types));
			switch (u->type)
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
					names.size());
			}
			break;
			}
		}
		ImGui::PopID();
	}

	void Builder::draw_source(CString label, List<char> & source)
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
						source.data(), 
						source.size(), 
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

	int32_t Builder::textEditCallback()
	{
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}