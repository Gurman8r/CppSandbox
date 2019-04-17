#include <MemeEditor/Project.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEngine/Resources.hpp>
#include <MemeEditor/Terminal.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/OS.hpp>

namespace ml
{
	struct Project::Funcs
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

	Project::Project()
		: GUI_Window("Project")
	{
	}

	Project::~Project()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Project::onEvent(const IEvent * value)
	{
	}

	bool Project::drawGui(bool * p_open)
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

			Funcs::Columns([&]()
			{
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.effects.getName().c_str(), [&]()
				{
					for (auto pair : ML_Res.effects)
					{
						Funcs::Group(pair.first.c_str(), [&](CString name, const Effect * e)
						{
							Funcs::Field("Name", [&](CString label)
							{
								ImGui::Text("%s", name);
							});
							if (const String file = ML_Res.effects.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.entities.getName().c_str(), [&]()
				{
					for (auto pair : ML_Res.entities)
					{
						Funcs::Group(pair.first.c_str(), [&](CString name, const Entity * e)
						{
							Funcs::Field("Name", [&](CString label)
							{
								ImGui::Text("%s", name);
							});
							if (const String file = ML_Res.entities.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.fonts.getName().c_str(), [&]()
				{
					for (auto pair : ML_Res.fonts)
					{
						Funcs::Group(pair.first.c_str(), [&](CString name, const Font * e)
						{
							Funcs::Field("Name", [&](CString label)
							{
								ImGui::Selectable(name);
							});
							Funcs::Field("Family", [&](CString label)
							{
								ImGui::Selectable(e->getInfo().family.c_str());
							});
							if (const String file = ML_Res.fonts.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.images.getName().c_str(), [&]()
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
							if (const String file = ML_Res.images.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.lua.getName().c_str(), [&]()
				{
					for (auto pair : ML_Res.lua)
					{
						Funcs::Group(pair.first.c_str(), [&](CString name, const LuaScript * e)
						{
							Funcs::Field("Name", [&](CString label)
							{
								ImGui::Text("%s", name);
							});
							if (const String file = ML_Res.lua.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.mats.getName().c_str(), [&]()
				{
					for (auto pair : ML_Res.mats)
					{
						Funcs::Group(pair.first.c_str(), [&](CString name, const Material * e)
						{
							Funcs::Field("Name", [&](CString label)
							{
								ImGui::Text("%s", name);
							});
							if (const String file = ML_Res.mats.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.meshes.getName().c_str(), [&]()
				{
					for (auto pair : ML_Res.meshes)
					{
						Funcs::Group(pair.first.c_str(), [&](CString name, const Mesh * e)
						{
							Funcs::Field("Name", [&](CString label)
							{
								ImGui::Text("%s", name);
							});
							if (const String file = ML_Res.meshes.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.models.getName().c_str(), [&]()
				{
					for (auto pair : ML_Res.models)
					{
						Funcs::Group(pair.first.c_str(), [&](CString name, const Model * e)
						{
							Funcs::Field("Name", [&](CString label)
							{
								ImGui::Text("%s", name);
							});
							if (const String file = ML_Res.models.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.plugins.getName().c_str(), [&]()
				{
					for (auto pair : ML_Res.plugins)
					{
						Funcs::Group(pair.first.c_str(), [&](CString name, const Plugin * e)
						{
							Funcs::Field("Name", [&](CString label)
							{
								ImGui::Text("%s", name);
							});
							if (const String file = ML_Res.plugins.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.scripts.getName().c_str(), [&]()
				{
					for (auto pair : ML_Res.scripts)
					{
						Funcs::Group(pair.first.c_str(), [&](CString name, const Script * e)
						{
							Funcs::Field("Name", [&](CString label)
							{
								ImGui::Text("%s", name);
							});
							if (const String file = ML_Res.scripts.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.shaders.getName().c_str(), [&]()
				{
					for (auto pair : ML_Res.shaders)
					{
						Funcs::Group(pair.first.c_str(), [&](CString name, const Shader * e)
						{
							Funcs::Field("Name", [&](CString label)
							{
								ImGui::Text("%s", name);
							});
							if (const String file = ML_Res.shaders.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.skyboxes.getName().c_str(), [&]()
				{
					for (auto pair : ML_Res.skyboxes)
					{
						Funcs::Group(pair.first.c_str(), [&](CString name, const Skybox * e)
						{
							Funcs::Field("Name", [&](CString label)
							{
								ImGui::Text("%s", name);
							});
							if (const String file = ML_Res.skyboxes.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.sounds.getName().c_str(), [&]()
				{
					for (auto pair : ML_Res.sounds)
					{
						Funcs::Group(pair.first.c_str(), [&](CString name, const Sound * e)
						{
							Funcs::Field("Name", [&](CString label)
							{
								ImGui::Text("%s", name);
							});
							if (const String file = ML_Res.sounds.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.sprites.getName().c_str(), [&]()
				{
					for (auto pair : ML_Res.sprites)
					{
						Funcs::Group(pair.first.c_str(), [&](CString name, const Sprite * e)
						{
							Funcs::Field("Name", [&](CString label)
							{
								ImGui::Text("%s", name);
							});
							if (const String file = ML_Res.sprites.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				Funcs::Group(ML_Res.textures.getName().c_str(), [&]()
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
							if (const String file = ML_Res.textures.getFile(name))
							{
								Funcs::Field("File", [&](CString label)
								{
									const String fName = ML_FS.getFileName(file);
									if (ImGui::Selectable(fName.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(file));
									}
								});
								Funcs::Field("Path", [&](CString label)
								{
									const String fPath = ML_FS.getFilePath(file);
									if (ImGui::Selectable(fPath.c_str()))
									{
										ML_OS.execute("open", ML_FS.pathTo(fPath));
									}
								});
							}

						}, pair.first.c_str(), pair.second);
					}
				});
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			});

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}