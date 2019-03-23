#include <MemeEditor/Hierarchy.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Hierarchy::Hierarchy()
		: base_type("Hierarchy")
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
		if (beginDraw(p_open))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
			ImGui::Columns(2);
			ImGui::Separator();

			struct funcs
			{
				static void ShowDummyObject(const char* prefix, int uid)
				{
					ImGui::PushID(uid); // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
					ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
					bool node_open = ImGui::TreeNode("Object", "%s_%u", prefix, uid);
					ImGui::NextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text("my sailor is rich");
					ImGui::NextColumn();
					if (node_open)
					{
						static float dummy_members[8] = { 0.0f,0.0f,1.0f,3.1416f,100.0f,999.0f };
						for (int i = 0; i < 8; i++)
						{
							ImGui::PushID(i); // Use field index as identifier.
							if (i < 2)
							{
								ShowDummyObject("Child", 424242);
							}
							else
							{
								// Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
								ImGui::AlignTextToFramePadding();
								ImGui::TreeNodeEx("Field", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet, "Field_%d", i);
								ImGui::NextColumn();
								ImGui::PushItemWidth(-1);
								if (i >= 5)
									ImGui::InputFloat("##value", &dummy_members[i], 1.0f);
								else
									ImGui::DragFloat("##value", &dummy_members[i], 0.01f);
								ImGui::PopItemWidth();
								ImGui::NextColumn();
							}
							ImGui::PopID();
						}
						ImGui::TreePop();
					}
					ImGui::PopID();
				}
			};

			// Iterate dummy objects with dummy members (all the same data)
			for (int obj_i = 0; obj_i < 3; obj_i++)
				funcs::ShowDummyObject("Object", obj_i);

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::PopStyleVar();
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}