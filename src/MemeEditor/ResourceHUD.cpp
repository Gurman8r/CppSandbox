#include <MemeEditor/ResourceHUD.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEditor/ResourceManager.hpp>
#include <MemeEditor/Terminal.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/OS.hpp>

namespace ml
{
	struct ResourceHUD::Funcs
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <typename Fun, typename ... Args>
		inline static void Tree(Fun fun, Args ... args)
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

		template <typename Fun, typename ... Args>
		inline static void Field(CString label, Fun fun, Args ... args)
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
				fun(label, (args)...);
			}
			ImGui::PopItemWidth();
			ImGui::NextColumn();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline static void FileInfoField(const String & fFile)
		{
			const String fPath = ML_FileSystem.getFilePath(fFile);
			const String fName = ML_FileSystem.getFileName(fFile);
			const String oPath = ML_FileSystem.pathTo(fPath);
			if (ImGui::Selectable(fPath.c_str()))
			{
				if (!ML_OS.execute("open", oPath))
				{
					ML_Terminal.printf("[ ERR ] Failed Opening \'%s\'", oPath.c_str());
				}
			}
			if (ImGui::Selectable(fName.c_str()))
			{
				if (!ML_OS.execute("open", oPath))
				{
					ML_Terminal.printf("[ ERR ] Failed Opening \'%s\'", oPath.c_str());
				}
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ResourceHUD::ResourceHUD()
		: base_type("Resources")
	{
	}

	ResourceHUD::~ResourceHUD()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void ResourceHUD::onEvent(const IEvent * value)
	{
	}

	bool ResourceHUD::draw(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("New"))
				{
					if (ImGui::MenuItem("Font")) {}
					if (ImGui::MenuItem("Image")) {}
					if (ImGui::MenuItem("Material")) {}
					if (ImGui::MenuItem("Mesh")) {}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			Funcs::Tree([&]()
			{
				Funcs::Group("Resources", [&]()
				{
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					Funcs::Group("Fonts", [&]()
					{
						for (auto pair : ML_Res.fonts)
						{
							Funcs::Group(pair.first.c_str(), [&](CString name, const Font * e)
							{
								Funcs::Field("Name", [&](CString label)
								{
									ImGui::Text("%s", name);
								});
								Funcs::Field("Family", [&](CString label)
								{
									ImGui::Text("%s", e->getInfo().family.c_str());
								});
								Funcs::Field("File", [&](CString label)
								{
									Funcs::FileInfoField(ML_Res.fonts.getFile(name));
								});

							}, pair.first.c_str(), pair.second);
						}
					});
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					Funcs::Group("Images", [&]()
					{
						for (auto pair : ML_Res.images)
						{
							Funcs::Group(pair.first.c_str(), [&](CString name, const Image * e)
							{
								Funcs::Field("Name", [&](CString label)
								{
									ImGui::Text("%s", name);
								});
								Funcs::Field("Size", [&](CString label)
								{
									ImGui::Text("%u x %u", e->width(), e->height());
								});
								Funcs::Field("File", [&](CString label)
								{
									Funcs::FileInfoField(ML_Res.images.getFile(name));
								});

							}, pair.first.c_str(), pair.second);
						}
					});
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					Funcs::Group("Materials", [&]()
					{
						for (auto pair : ML_Res.mats)
						{
							Funcs::Group(pair.first.c_str(), [&](CString name, const Material * e)
							{
								Funcs::Field("Name", [&](CString label)
								{
									ImGui::Text("%s", name);
								});
								Funcs::Field("File", [&](CString label)
								{
									Funcs::FileInfoField(ML_Res.mats.getFile(name));
								});

							}, pair.first.c_str(), pair.second);
						}
					});
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					Funcs::Group("Meshes", [&]()
					{
						for (auto pair : ML_Res.meshes)
						{
							Funcs::Group(pair.first.c_str(), [&](CString name, const Mesh * e)
							{
								Funcs::Field("Name", [&](CString label)
								{
									ImGui::Text("%s", name);
								});
								Funcs::Field("File", [&](CString label)
								{
									Funcs::FileInfoField(ML_Res.meshes.getFile(name));
								});

							}, pair.first.c_str(), pair.second);
						}
					});
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					Funcs::Group("Models", [&]()
					{
						for (auto pair : ML_Res.models)
						{
							Funcs::Group(pair.first.c_str(), [&](CString name, const Model * e)
							{
								Funcs::Field("Name", [&](CString label)
								{
									ImGui::Text("%s", name);
								});

							}, pair.first.c_str(), pair.second);
						}
					});
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					Funcs::Group("Scripts", [&]()
					{
						for (auto pair : ML_Res.scripts)
						{
							Funcs::Group(pair.first.c_str(), [&](CString name, const Script * e)
							{
								Funcs::Field("Name", [&](CString label)
								{
									ImGui::Text("%s", name);
								});
								Funcs::Field("File", [&](CString label)
								{
									Funcs::FileInfoField(ML_Res.scripts.getFile(name));
								});

							}, pair.first.c_str(), pair.second);
						}
					});
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					Funcs::Group("Shaders", [&]()
					{
						for (auto pair : ML_Res.shaders)
						{
							Funcs::Group(pair.first.c_str(), [&](CString name, const Shader * e)
							{
								Funcs::Field("Name", [&](CString label)
								{
									ImGui::Text("%s", name);
								});
								Funcs::Field("File", [&](CString label)
								{
									Funcs::FileInfoField(ML_Res.shaders.getFile(name));
								});

							}, pair.first.c_str(), pair.second);
						}
					});
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					Funcs::Group("Skyboxes", [&]()
					{
						for (auto pair : ML_Res.skyboxes)
						{
							Funcs::Group(pair.first.c_str(), [&](CString name, const Skybox * e)
							{
								Funcs::Field("Name", [&](CString label)
								{
									ImGui::Text("%s", name);
								});
								Funcs::Field("File", [&](CString label)
								{
									Funcs::FileInfoField(ML_Res.skyboxes.getFile(name));
								});

							}, pair.first.c_str(), pair.second);
						}
					});
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					Funcs::Group("Sounds", [&]()
					{
						for (auto pair : ML_Res.sounds)
						{
							Funcs::Group(pair.first.c_str(), [&](CString name, const Sound * e)
							{
								Funcs::Field("Name", [&](CString label)
								{
									ImGui::Text("%s", name);
								});
								Funcs::Field("File", [&](CString label)
								{
									Funcs::FileInfoField(ML_Res.sounds.getFile(name));
								});

							}, pair.first.c_str(), pair.second);
						}
					});
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					Funcs::Group("Sprites", [&]()
					{
						for (auto pair : ML_Res.sprites)
						{
							Funcs::Group(pair.first.c_str(), [&](CString name, const Sprite * e)
							{
								Funcs::Field("Name", [&](CString label)
								{
									ImGui::Text("%s", name);
								});

							}, pair.first.c_str(), pair.second);
						}
					});
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
					Funcs::Group("Textures", [&]()
					{
						for (auto pair : ML_Res.textures)
						{
							Funcs::Group(pair.first.c_str(), [&](CString name, const Texture * e)
							{
								Funcs::Field("Name", [&](CString label)
								{
									ImGui::Text("%s", name);
								});
								Funcs::Field("Size", [&](CString label)
								{
									ImGui::Text("%u x %u", e->width(), e->height());
								});
								Funcs::Field("Real Size", [&](CString label)
								{
									ImGui::Text("%u x %u", e->realWidth(), e->realHeight());
								});
								Funcs::Field("File", [&](CString label)
								{
									Funcs::FileInfoField(ML_Res.textures.getFile(name));
								});

							}, pair.first.c_str(), pair.second);
						}
					});
					/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				});
			});

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}