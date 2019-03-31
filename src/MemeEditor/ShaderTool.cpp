#include <MemeEditor/ShaderTool.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	struct ShaderTool::Funcs
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <typename Fun, typename ... Args>
		inline static void Columns(Fun fun, Args ... args)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 2, 2 });
			ImGui::Columns(2);
			ImGui::Separator();
			{
				fun((args)...);
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::PopStyleVar();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <typename Fun, typename ... Args>
		inline static void Group(CString label, Fun fun, Args ... args)
		{
			ImGui::PushID(label);
			ImGui::AlignTextToFramePadding();
			bool node_open;
			{
				node_open = ImGui::TreeNode("ResourceHUD_Group", "%s", label);
			}
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();
			{
				ImGui::Text("");
			}
			ImGui::NextColumn();
			if (node_open)
			{
				fun((args)...);

				ImGui::TreePop();
			}
			ImGui::PopID();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <typename Fun, typename ... Args>
		inline static void Field(CString label, Fun fun, Args ... args)
		{
			ImGui::AlignTextToFramePadding();
			ImGui::TreeNodeEx(
				"ResourceHUD_Field",
				ImGuiTreeNodeFlags_Leaf |
				ImGuiTreeNodeFlags_NoTreePushOnOpen |
				ImGuiTreeNodeFlags_Bullet,
				"%s",
				label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);
			{
				fun(label, (args)...);
			}
			ImGui::PopItemWidth();
			ImGui::NextColumn();
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ShaderTool::ShaderTool()
		: GUI_Window("Shader Tool")
		, m_items()
		, m_curItem(-1)
	{
	}

	ShaderTool::~ShaderTool()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void ShaderTool::onEvent(const IEvent * value)
	{
	}

	bool ShaderTool::draw(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar))
		{
			/* * * * * * * * * * * * * * * * * * * * */
			
			draw_menu();

			ImGui::Text("%s (WIP)", title());

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void ShaderTool::draw_menu()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("New"))
				{
					/* * * * * * * * * * * * * * * * * * * * */

					auto newItem = [&](
						const String & name, 
						const Item::Type type,
						const UniformSet & uni)
					{
						m_items.push_back(Item(
							String("New {0} {1}").format(name, m_items.size()).c_str(),
							type,
							uni
						));
					};

					/* * * * * * * * * * * * * * * * * * * * */

					if (ImGui::MenuItem("Vertex"))
					{
						newItem("Vertex", Item::Vertex, {
								Uniform("Vert.model",	Uniform::Mat4),
								Uniform("Vert.view",	Uniform::Mat4),
								Uniform("Vert.proj",	Uniform::Mat4),
							});
					}
					if (ImGui::MenuItem("Fragment"))
					{
						newItem("Fragment", Item::Fragment, {
								Uniform("Frag.mainCol", Uniform::Vec4),
								Uniform("Frag.mainTex", Uniform::Tex2D),
							});
					}
					if (ImGui::MenuItem("Geometry"))
					{
						newItem("Geometry", Item::Geometry, {
							});
					}

					/* * * * * * * * * * * * * * * * * * * * */

					ImGui::EndMenu();

					/* * * * * * * * * * * * * * * * * * * * */
				}

				if (ImGui::MenuItem("Save"))
				{
					if (Item * item = getSelected())
					{
						item->doc.dirty = false;
					}
				}

				ImGui::EndMenu();
			}

			ImGui::Text("| Item: %i", m_curItem);

			ImGui::EndMenuBar();
		}
	}

	void ShaderTool::draw_tabs()
	{
		ImGui::BeginGroup();
		{
			if (ImGui::BeginTabBar("Tabs"))
			{
				m_curItem = -1;

				for (size_t i = 0; i < m_items.size(); i++)
				{
					if (Item * item = getItem(i))
					{
						if (ImGui::BeginTabBar("Item Tabs"))
						{
							edit_uniforms_tab(&item->uni);

							if (edit_document_tab(&item->doc))
							{
								m_curItem = (int32_t)(i);
							}
							else
							{
								m_items.erase(m_items.begin() + i);
								break;
							}

							ImGui::EndTabBar();
						}
					}
				}

				ImGui::EndTabBar();
			}
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool ShaderTool::edit_document_tab(Document * value)
	{
		if (!value) { return false; }

		ImGuiTabItemFlags flags = (value->dirty ? ImGuiTabItemFlags_UnsavedDocument : 0);
		if (ImGui::BeginTabItem(value->name, &value->open, flags))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			Document::Name name;
			if (edit_document_name(name, value))
			{
				strcpy(value->name, name);
			}
			ImGui::Separator();

			/* * * * * * * * * * * * * * * * * * * * */

			if (edit_document_data(value))
			{
				value->dirty = true;
			}
			ImGui::Separator();

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::Text("%u/%u", value->sizeUsed(), value->DataSize);

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::EndTabItem();
		}
		return value->open;
	}

	bool ShaderTool::edit_document_name(char * buf, Document * value)
	{
		strcpy(buf, value->name);
		return ImGui::InputText(
			("Name"),
			(buf),
			(Document::NameSize),
			(ImGuiInputTextFlags_EnterReturnsTrue),
			(NULL),
			(NULL));
	}

	bool ShaderTool::edit_document_data(Document * value)
	{
		return ImGui::InputTextMultiline(
			("##Data"),
			(value->data),
			(IM_ARRAYSIZE(value->data)),
			(ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16)),
			(ImGuiInputTextFlags_AllowTabInput),
			(NULL),
			(NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool ShaderTool::edit_uniforms_tab(UniformSet * value)
	{
		if (ImGui::BeginTabItem("Uniforms"))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			edit_uniforms_list(value);

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::EndTabItem();
		}
		return true;
	}

	bool ShaderTool::edit_uniforms_list(UniformSet * value)
	{
		if (!value) { return false; }

		if (ImGui::BeginChild("Uniform List", { 224, 0 }, true))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			m_curUniform = -1;

			/* * * * * * * * * * * * * * * * * * * * */

			int32_t i = 0;
			for (auto pair : (*value))
			{
				if (ImGui::Selectable(
					pair.second.name.c_str(),
					m_curUniform == i,
					ImGuiSelectableFlags_AllowDoubleClick
				))
				{
					m_curUniform = i;
				}
			}

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::BeginGroup();
			{
				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::Button("Up"))
				{
					if (!value->empty() && m_curUniform > 0)
					{
						//std::swap(value[m_curUniform], value[m_curUniform - 1]);
						//m_curUniform--;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Dn"))
				{
					if (m_curUniform + 1 < (int32_t)value->size())
					{
						//std::swap(value[m_curUniform], value[m_curUniform + 1]);
						//m_curUniform++;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("New"))
				{
					//value->push_back(Uniform("new_uniform"));
					//m_curUniform = value->size() - 1;
				}
				ImGui::SameLine();
				if (ImGui::Button("Ins"))
				{
					if (value->size() > 0)
					{
						//value->insert(value->begin() + m_curUniform, Uniform("new_uniform"));
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Dup"))
				{
					if (value->size() > 0)
					{
						//value->push_back(Uniform(value[m_curUniform]));
						//m_curUniform = value->size() - 1;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Del"))
				{
					if (value->size() > 0)
					{
						//value->erase(value->begin() + m_curUniform);
						//m_curUniform = (m_curUniform > 0 ? m_curUniform - 1 : value->size() - 1);
					}
				}

				/* * * * * * * * * * * * * * * * * * * * */
			}
			ImGui::EndGroup();

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::EndChild();
		}
		return true;
	}

	void ShaderTool::draw_uniform_list_buttons(UniformSet * value)
	{
		ImGui::BeginGroup();
		{
			if (ImGui::Button("Up"))
			{
				if (!value->empty() && m_curUniform > 0)
				{
					//std::swap(value[m_curUniform], value[m_curUniform - 1]);
					//m_curUniform--;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Dn"))
			{
				if (m_curUniform + 1 < (int32_t)value->size())
				{
					//std::swap(value[m_curUniform], value[m_curUniform + 1]);
					//m_curUniform++;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("New"))
			{
				//value->push_back(Uniform("new_uniform"));
				//m_curUniform = value->size() - 1;
			}
			ImGui::SameLine();
			if (ImGui::Button("Ins"))
			{
				if (value->size() > 0)
				{
					//value->insert(value->begin() + m_curUniform, Uniform("new_uniform"));
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Dup"))
			{
				if (value->size() > 0)
				{
					//value->push_back(Uniform(value[m_curUniform]));
					//m_curUniform = value->size() - 1;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Del"))
			{
				if (value->size() > 0)
				{
					//value->erase(value->begin() + m_curUniform);
					//m_curUniform = (m_curUniform > 0 ? m_curUniform - 1 : value->size() - 1);
				}
			}
		}
		ImGui::EndGroup();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}