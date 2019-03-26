#include <MemeEditor/ProjectView.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEditor/ResourceManager.hpp>

namespace ml
{
	struct ProjectView::Funcs
	{
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
			fun(label);
			ImGui::PopItemWidth();
			ImGui::NextColumn();
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
	};
}

namespace ml
{
	ProjectView::ProjectView()
		: base_type("Project View")
	{
	}

	ProjectView::~ProjectView()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void ProjectView::onEvent(const IEvent * value)
	{
	}

	bool ProjectView::draw(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::EndMenuBar();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
			ImGui::Columns(2);
			ImGui::Separator();
			{
				Funcs::Group("Resources", [&]() 
				{
					Funcs::Group("Fonts", [&]()
					{
						for (auto pair : ML_Res.fonts)
						{
							Funcs::Field(pair.first.c_str(), [&](CString label) 
							{
								ImGui::Text(label);
							});
						}
					});

					Funcs::Group("Images", [&]()
					{
						for (auto pair : ML_Res.fonts)
						{
							Funcs::Field(pair.first.c_str(), [&](CString label)
							{
								ImGui::Text(label);
							});
						}
					});

					Funcs::Group("Materials", [&]()
					{
						for (auto pair : ML_Res.mats)
						{
							Funcs::Field(pair.first.c_str(), [&](CString label)
							{
								ImGui::Text(label);
							});
						}
					});

					Funcs::Group("Meshes", [&]()
					{
						for (auto pair : ML_Res.meshes)
						{
							Funcs::Field(pair.first.c_str(), [&](CString label)
							{
								ImGui::Text(label);
							});
						}
					});

					Funcs::Group("Models", [&]()
					{
						for (auto pair : ML_Res.models)
						{
							Funcs::Field(pair.first.c_str(), [&](CString label)
							{
								ImGui::Text(label);
							});
						}
					});

					Funcs::Group("Scripts", [&]()
					{
						for (auto pair : ML_Res.scripts)
						{
							Funcs::Field(pair.first.c_str(), [&](CString label)
							{
								ImGui::Text(label);
							});
						}
					});

					Funcs::Group("Shaders", [&]()
					{
						for (auto pair : ML_Res.shaders)
						{
							Funcs::Field(pair.first.c_str(), [&](CString label)
							{
								ImGui::Text(label);
							});
						}
					});

					Funcs::Group("Skyboxes", [&]()
					{
						for (auto pair : ML_Res.skyboxes)
						{
							Funcs::Field(pair.first.c_str(), [&](CString label)
							{
								ImGui::Text(label);
							});
						}
					});

					Funcs::Group("Sounds", [&]()
					{
						for (auto pair : ML_Res.sounds)
						{
							Funcs::Field(pair.first.c_str(), [&](CString label)
							{
								ImGui::Text(label);
							});
						}
					});

					Funcs::Group("Sprites", [&]()
					{
						for (auto pair : ML_Res.sprites)
						{
							Funcs::Field(pair.first.c_str(), [&](CString label)
							{
								ImGui::Text(label);
							});
						}
					});

					Funcs::Group("Textures", [&]()
					{
						for (auto pair : ML_Res.textures)
						{
							Funcs::Field(pair.first.c_str(), [&](CString label)
							{
								ImGui::Text(label);
							});
						}
					});
				});
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::PopStyleVar();
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}