#include <MemeEditor/ProjectView.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEditor/Terminal.hpp>
#include <MemeEditor/GUI.hpp>
#include <MemeEngine/Resources.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/OS.hpp>
#include <MemeCore/CoreEvents.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeGraphics/Renderer.hpp>

namespace ml
{
	struct ProjectView::Funcs
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Fun,
			class ... Args
		> inline static void Columns(Fun fun, Args ... args)
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

		template <
			class Fun,
			class ... Args
		> inline static void Group(CString label, Fun fun, Args ... args)
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

		template <
			class Fun, 
			class ... Args
		> inline static void Field(CString label, Fun fun, Args ... args)
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

	ProjectView::ProjectView()
		: GUI_Window("Project View")
	{
	}

	ProjectView::~ProjectView()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void ProjectView::onEvent(const IEvent * value)
	{
	}

	bool ProjectView::drawGui(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("New (WIP)"))
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
				draw_effects();
				draw_entities();
				draw_fonts();
				draw_images();
				draw_lua();
				draw_mats();
				draw_meshes();
				draw_models();
				draw_plugins();
				draw_scripts();
				draw_shaders();
				draw_skyboxes();
				draw_sounds();
				draw_sprites();
				draw_textures();
			});

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void ProjectView::draw_effects()
	{
		Funcs::Group(ML_Res.effects.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.effects)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_entities()
	{
		Funcs::Group(ML_Res.entities.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.entities)
			{
				Funcs::Group(pair.first.c_str(), [&](CString name, Entity * e)
				{
					Funcs::Field("Name", [&](CString)
					{
						ImGui::Text("%s", name);
					});

					Funcs::Group("Components", [&](CString) 
					{
						// Transform
						if (Transform * t = e->get<Transform>())
						{
							Funcs::Field("Transform", [&](CString)
							{
								vec3f pos = t->getPosition();
								if (GUI::EditVec3f("Position##Transform", pos))
								{
									t->setPosition(pos);
								}

								vec3f scl = t->getScale();
								if (GUI::EditVec3f("Scale##Transform", scl))
								{
									t->setScale(scl);
								}

								quat rot = t->getRotation();
								if (GUI::EditVec4f("Rotation##Transform", rot))
								{
									t->setRotation(rot);
								}
							});
						}

						// Renderer
						if (Renderer * r = e->get<Renderer>())
						{
							Funcs::Field("Renderer", [&](CString)
							{
								ImGui::Text("OK");
							});
						}

					}, (CString)(NULL));

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_fonts()
	{
		Funcs::Group(ML_Res.fonts.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.fonts)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_images()
	{
		Funcs::Group(ML_Res.images.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.images)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_lua()
	{
		Funcs::Group(ML_Res.lua.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.lua)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_mats()
	{
		Funcs::Group(ML_Res.mats.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.mats)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_meshes()
	{
		Funcs::Group(ML_Res.meshes.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.meshes)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_models()
	{
		Funcs::Group(ML_Res.models.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.models)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_plugins()
	{
		Funcs::Group(ML_Res.plugins.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.plugins)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_scripts()
	{
		Funcs::Group(ML_Res.scripts.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.scripts)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_shaders()
	{
		Funcs::Group(ML_Res.shaders.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.shaders)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_skyboxes()
	{
		Funcs::Group(ML_Res.skyboxes.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.skyboxes)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_sounds()
	{
		Funcs::Group(ML_Res.sounds.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.sounds)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_sprites()
	{
		Funcs::Group(ML_Res.sprites.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.sprites)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ProjectView::draw_textures()
	{
		Funcs::Group(ML_Res.textures.getName().c_str(), [&]()
		{
			for (auto & pair : ML_Res.textures)
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
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString label)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_OpenEvent(ML_FS.pathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	/* * * * * * * * * * * * * * * * * * * * */
}