#include <MemeEditor/Hierarchy.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEditor/ResourceManager.hpp>

namespace ml
{
	struct Hierarchy::Funcs
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <typename Fun>
		static void Tree(Fun fun)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 2, 2 });
			ImGui::Columns(2);
			ImGui::Separator();
			{
				fun();
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::PopStyleVar();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <typename Fun, typename ... Args>
		static void Group(CString label, Fun fun, Args ... args)
		{
			ImGui::PushID(label);
			ImGui::AlignTextToFramePadding();
			bool node_open;
			{
				node_open = ImGui::TreeNode("Object", "%s", label);
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

		template <typename Fun>
		static void Field(CString label, Fun fun)
		{
			ImGui::AlignTextToFramePadding();
			ImGui::TreeNodeEx(
				"Field",
				ImGuiTreeNodeFlags_Leaf |
				ImGuiTreeNodeFlags_NoTreePushOnOpen |
				ImGuiTreeNodeFlags_Bullet,
				"%s",
				label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);
			{
				fun(label);
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
		: base_type("Hierarchy")
		, m_objects({ GameObject("Object A"), GameObject("Object B"), GameObject("Object C") })
	{
	}

	Hierarchy::~Hierarchy()
	{
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
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("New"))
				{
					if (ImGui::MenuItem("Game Object")) { }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			Funcs::Tree([&]() 
			{
				for (size_t i = 0; i < m_objects.size(); i++)
				{
					Funcs::Group(m_objects[i].name, [](GameObject * value)
					{
						/* * * * * * * * * * * * * * * * * * * * */

						Funcs::Field("Name", [&](CString label)
						{
							ImGui::InputText(
								label,
								value->name,
								IM_ARRAYSIZE(value->name),
								ImGuiInputTextFlags_AllowTabInput,
								NULL,
								NULL);
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
									GUI::EditVec3f(label, scale);
								});

								Funcs::Field("Orientation", [&](CString label)
								{
									GUI::EditVec4f(label, orient);
								});

								Funcs::Field("Translation", [&](CString label)
								{
									GUI::EditVec3f(label, trans);
								});

								Funcs::Field("Skew", [&](CString label)
								{
									GUI::EditVec3f(label, skew);
								});

								Funcs::Field("Perspective", [&](CString label)
								{
									GUI::EditVec4f(label, persp);
								});
							}
						});

						/* * * * * * * * * * * * * * * * * * * * */

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
								vec4f color = value->color;
								ImGui::ColorEdit4(label, &color[0]);
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

								const ResourceManager::TextureMap & textures = ML_Res.textures;
								List<String> names;
								for (auto pair : textures)
								{
									names.push_back(pair.first);
								}

								static int32_t index = 0;
								if (ImGui::Combo(
									label,
									&index,
									vector_getter,
									(void *)(&names),
									(int32_t)names.size()))
								{
									// changed
								}
							});
						});

						/* * * * * * * * * * * * * * * * * * * * */
					}, &m_objects[i]);
				}
			});

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}