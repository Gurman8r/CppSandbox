#include <MemeEditor/Hierarchy.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEditor/ResourceManager.hpp>

namespace ml
{
	struct Hierarchy::Funcs
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
				node_open = ImGui::TreeNode("Hierarchy_Group", "%s", label);
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
				"Hierarchy_Field",
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

	Hierarchy::Hierarchy()
		: GUI_Window("Hierarchy")
		, m_objects()
	{
	}

	Hierarchy::~Hierarchy()
	{
		for (auto pair : m_objects)
		{
			delete pair.second;
			pair.second = NULL;
		}
		m_objects.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Hierarchy::onEvent(const IEvent * value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Hierarchy::draw(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar))
		{
			// Menu Bar
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("New"))
				{
					if (ImGui::MenuItem("Game Object")) 
					{
						if (GameObject * obj = newObject(GameObject(
							String("New Game Object ({0})").format(
								m_objects.size()
							).c_str()
						)))
						{

						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			// Hierarchy
			/* * * * * * * * * * * * * * * * * * * * */
			Funcs::Columns([&]() 
			{
				for (auto pair : m_objects)
				{
					Funcs::Group(pair.second->name, [&](GameObject * value)
					{
						/* * * * * * * * * * * * * * * * * * * * */

						Funcs::Group("Properties", [&]() 
						{
							Funcs::Field("Name", [&](CString label)
							{
								GameObject::Name buf;
								if (edit_object_name(buf, value))
								{
									strcpy(value->name, buf);
								}
							});
						});

						/* * * * * * * * * * * * * * * * * * * * */

						Funcs::Group("Transform", [&]()
						{
							vec3f scale;
							quat  orient;
							vec3f trans;
							vec3f skew;
							vec4f persp;

							if (value->transform.decompose(scale, orient, trans, skew, persp))
							{
								Funcs::Field("Scale", [&](CString label)
								{
									GUI::EditVec3f("##Scale", scale);
								});

								Funcs::Field("Orientation", [&](CString label)
								{
									GUI::EditVec4f("##Orientation", orient);
								});

								Funcs::Field("Translation", [&](CString label)
								{
									GUI::EditVec3f("##Translation", trans);
								});

								Funcs::Field("Skew", [&](CString label)
								{
									GUI::EditVec3f("##Skew", skew);
								});

								Funcs::Field("Perspective", [&](CString label)
								{
									GUI::EditVec4f("##Perspective", persp);
								});
							}
						});

						Funcs::Group("Vertex", [&]()
						{
							Funcs::Group("MVP", [&]()
							{
								ImGui::Separator();
								Funcs::Field("Vert.proj", [&](CString label)
								{
									static mat4f temp;
									GUI::EditMat4f(label, temp);
								});
								ImGui::Separator();
								Funcs::Field("Vert.view", [&](CString label)
								{
									static mat4f temp;
									GUI::EditMat4f(label, temp);
								});
								ImGui::Separator();
								Funcs::Field("Vert.model", [&](CString label)
								{
									static mat4f temp;
									GUI::EditMat4f(label, temp);
								});
								ImGui::Separator();
							});
						});

						Funcs::Group("Fragment", [&]()
						{
							Funcs::Field("Frag.mainCol", [&](CString label)
							{
								if (ImGui::ColorEdit4(
									(("##" + String(label)).c_str()),
									(&value->color[0])))
								{
									// changed
								}
							});

							Funcs::Field("Frag.mainTex", [&](CString label)
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

								const List<String> & keys = ML_Res.textures.getKeys();

								static int32_t index = 0;

								if (ImGui::Combo(
									(("##" + String(label)).c_str()),
									&index,
									vector_getter,
									(void *)(&keys),
									(int32_t)keys.size()))
								{
								}
							});
						});

						/* * * * * * * * * * * * * * * * * * * * */
					}, pair.second);
				}
			});

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	GameObject * Hierarchy::getObject(const String & name)
	{
		iterator it;
		return (((it = m_objects.find(name)) != m_objects.end())
			? (it->second)
			: (NULL));
	}

	GameObject * Hierarchy::newObject(const GameObject & value)
	{
		return ((m_objects.find(value.name) == m_objects.end())
			? (m_objects[value.name] = new GameObject(value))
			: (NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Hierarchy::edit_object_name(char * buf, GameObject * value)
	{
		strcpy(buf, value->name);
		return ImGui::InputText(
			("##Name"),
			(buf),
			(GameObject::NameSize),
			(ImGuiInputTextFlags_EnterReturnsTrue),
			(NULL),
			(NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}