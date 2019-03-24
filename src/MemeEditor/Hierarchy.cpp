#include <MemeEditor/Hierarchy.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Hierarchy::Hierarchy()
		: base_type("Hierarchy")
		, m_objects({ GameObject() })
	{
	}

	Hierarchy::~Hierarchy()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Hierarchy::onEvent(const IEvent * value)
	{
	}

	bool Hierarchy::draw(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Create"))
				{
					if (ImGui::MenuItem("Game Object")) { }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
			ImGui::Columns(2);
			ImGui::Separator();

			struct funcs
			{
				static void ShowGameObject(GameObject * value, int32_t uid)
				{
					ImGui::PushID(uid);
					ImGui::AlignTextToFramePadding();
					bool node_open = ImGui::TreeNode("Object", "%s_%u", "GameObject", uid);
					ImGui::NextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Game Object Name");
					ImGui::NextColumn();
					if (node_open)
					{
						auto propertyField = [](CString label, auto fun)
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
							fun((String("##") + label).c_str());
							ImGui::PopItemWidth();
							ImGui::NextColumn();
						};

						vec3f scale;
						quat  orient;
						vec3f trans;
						vec3f skew;
						vec4f persp;

						if (value->transform.decompose(scale, orient, trans, skew, persp))
						{
							propertyField("Position", [&](CString label)
							{
								vec3f temp = value->transform.getPosition();
								GUI::EditVec3f(label, temp);
							});

							propertyField("Scale", [&](CString label)
							{
								GUI::EditVec3f(label, scale);
							});

							propertyField("Orient", [&](CString label)
							{
								GUI::EditVec4f(label, orient);
							});

							propertyField("Trans", [&](CString label)
							{
								GUI::EditVec3f(label, trans);
							});

							propertyField("Skew", [&](CString label)
							{
								GUI::EditVec3f(label, skew);
							});

							propertyField("Persp", [&](CString label)
							{
								GUI::EditVec4f(label, persp);
							});
						}
						ImGui::TreePop();
					}
					ImGui::PopID();
				}
			};

			// Iterate dummy objects with dummy members (all the same data)
			for (size_t i = 0; i < m_objects.size(); i++)
			{
				funcs::ShowGameObject(&m_objects[i], (int32_t)i);
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::PopStyleVar();
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}