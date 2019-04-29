#include <MemeEditor/ResourceView.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEditor/ImGui_Helper.hpp>
#include <MemeEditor/Terminal.hpp>
#include <MemeEditor/GUI.hpp>
#include <MemeEngine/Resources.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/OS.hpp>
#include <MemeCore/CoreEvents.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeGraphics/Renderer.hpp>
#include <MemeGraphics/Light.hpp>
#include <MemePhysics/Rigidbody.hpp>
#include <MemePhysics/Particle.hpp>
#include <MemeGraphics/Camera.hpp>
#include <MemeGraphics/Uni.hpp>

namespace ml
{
	struct ResourceView::Funcs
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
				node_open = ImGui::TreeNode("ResourceView_Group", "%s", label);
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
				"ResourceView_Field",
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

	ResourceView::ResourceView()
		: GUI_Window("Resources")
	{
	}

	ResourceView::~ResourceView()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void ResourceView::onEvent(const IEvent * value)
	{
	}

	bool ResourceView::drawGui(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("New (WIP)"))
				{
					if (ImGui::MenuItem("Effect"	)) { /**/ }
					if (ImGui::MenuItem("Entity"	)) { /**/ }
					if (ImGui::MenuItem("Font"		)) { /**/ }
					if (ImGui::MenuItem("Image"		)) { /**/ }
					if (ImGui::MenuItem("Lua"		)) { /**/ }
					if (ImGui::MenuItem("Material"	)) { /**/ }
					if (ImGui::MenuItem("Mesh"		)) { /**/ }
					if (ImGui::MenuItem("Model"		)) { /**/ }
					if (ImGui::MenuItem("Plugin"	)) { /**/ }
					if (ImGui::MenuItem("Script"	)) { /**/ }
					if (ImGui::MenuItem("Shader"	)) { /**/ }
					if (ImGui::MenuItem("Skybox"	)) { /**/ }
					if (ImGui::MenuItem("Sound"		)) { /**/ }
					if (ImGui::MenuItem("Sprite"	)) { /**/ }
					if (ImGui::MenuItem("Texture"	)) { /**/ }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			Funcs::Columns([&]()
			{
				draw_effects	();
				draw_entities	();
				draw_fonts		();
				draw_images		();
				draw_lua		();
				draw_meshes		();
				draw_materials	();
				draw_models		();
				draw_plugins	();
				draw_scripts	();
				draw_shaders	();
				draw_skyboxes	();
				draw_sounds		();
				draw_sprites	();
				draw_textures	();
			});

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void ResourceView::draw_effects()
	{
		if (ML_Res.effects.empty()) return;

		Funcs::Group(ML_Res.effects.name().c_str(), [&]()
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
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_entities()
	{
		if (ML_Res.entities.empty()) return;

		Funcs::Group(ML_Res.entities.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.entities)
			{
				Funcs::Group(pair.first.c_str(), [&](CString name, Entity * ent)
				{
					// Name
					/* * * * * * * * * * * * * * * * * * * * */
					Funcs::Field("Name", [&](CString)
					{
						ImGui::Text("%s", name);
					});

					// Components
					/* * * * * * * * * * * * * * * * * * * * */
					{
						// Camera
						/* * * * * * * * * * * * * * * * * * * * */
						if (Camera * camera = ent->get<Camera>())
						{
							Funcs::Group("Camera", [&]()
							{
								Funcs::Field("Color", [&](CString)
								{
									ImGui::ColorEdit4("##Color##Camera", &camera->color[0]);
								});
								Funcs::Field("FOV", [&](CString)
								{
									ImGui::DragFloat("##FOV##Camera", &camera->fov, 0.5f, 10.f, 100.f);
								});
								Funcs::Group("Perspective", [&]() 
								{
									Funcs::Field("Near", [&](CString)
									{
										ImGui::DragFloat("##pNear##Camera", &camera->pNear, 0.5f, 10.f, camera->pFar - 1.f);
									});
									Funcs::Field("Far", [&](CString)
									{
										ImGui::DragFloat("##pFar##Camera", &camera->pFar, 0.5f, camera->pNear + 1.f, 1000.0f);
									});
									Funcs::Field("Matrix", [&](CString)
									{
										mat4 mat = camera->getPerspMatrix();
										GUI::EditMat4f("##O##Matrix##Camera", mat);
									});
								});
								Funcs::Group("Orthographic", [&]()
								{
									Funcs::Field("Near", [&](CString)
									{
										ImGui::DragFloat("##oNear##Camera", &camera->oNear, 0.5f, 10.f, camera->oFar - 1.f);
									});
									Funcs::Field("Far", [&](CString)
									{
										ImGui::DragFloat("##oFar##Camera", &camera->oFar, 0.5f, camera->oNear + 1.f, 100.f);
									});
									Funcs::Field("Matrix", [&](CString)
									{
										mat4 mat = camera->getOrthoMatrix();
										GUI::EditMat4f("##O##Matrix##Camera", mat);
									});
								});
							});
						}

						// Light
						/* * * * * * * * * * * * * * * * * * * * */
						if (Light * light = ent->get<Light>())
						{
							Funcs::Group("Light", [&]()
							{
								Funcs::Field("Color", [&](CString) 
								{
									ImGui::ColorEdit4("##Color##Light", &light->color[0]);
								});
							});
						}

						// Renderer
						/* * * * * * * * * * * * * * * * * * * * */
						if (Renderer * renderer = ent->get<Renderer>())
						{
							Funcs::Group("Renderer", [&]()
							{
								// Model
								Funcs::Field("Model", [&](CString)
								{
									List<String> keys = ML_Res.models.keys();
									int32_t index = ML_Res.models.getIndexOf((Model *)(renderer->drawable()));
									if (ImGui::Combo(
										"##Model##Renderer",
										&index,
										ImGui_Helper::vector_getter,
										static_cast<void *>(&keys),
										(int32_t)(keys.size())))
									{
										if (const Model * value = ML_Res.models.getByIndex(index))
										{
											renderer->drawable() = value;
										}
									}
								});
								
								// States
								//Funcs::Group("States", [&]() {});
								for (auto & pair : renderer->states())
								{
									switch (pair.first)
									{
									case GL::AlphaTest:
										Funcs::Field("Alpha Test", [&](CString)
										{
											ImGui::Checkbox("##AlphaTest##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::Blend:
										Funcs::Field("Blend", [&](CString)
										{
											ImGui::Checkbox("##Blend##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::CullFace:
										Funcs::Field("Cull Face", [&](CString)
										{
											ImGui::Checkbox("##CullFace##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::DepthTest:
										Funcs::Field("Depth Test", [&](CString)
										{
											ImGui::Checkbox("##DepthTest##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::Multisample:
										Funcs::Field("Multisample", [&](CString)
										{
											ImGui::Checkbox("##Multisample##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::Texture2D:
										Funcs::Field("Texture2D", [&](CString)
										{
											ImGui::Checkbox("##Texture2D##States", (bool *)(pair.second.ptr()));
										});
										break;
									}
								}
							});
						}

						// Rigidbody
						/* * * * * * * * * * * * * * * * * * * * */
						if (Rigidbody * rb = ent->get<Rigidbody>())
						{
							Funcs::Group("Rigidbody", [&]()
							{
								Funcs::Field("Index", [&](CString)
								{
									ImGui::Text("%i", rb->index());
								});

								Funcs::Field("Collider", [&](CString)
								{
									ImGui::Text("%s", rb->collider() ? "OK" : "NULL");
								});

								Funcs::Field("Particle", [&](CString)
								{
									ImGui::Text("%s", rb->particle() ? "OK" : "NULL");
								});

								Funcs::Field("Transform", [&](CString)
								{
									ImGui::Text("%s", rb->transform() ? "OK" : "NULL");
								});
							});
						}

						// Transform
						/* * * * * * * * * * * * * * * * * * * * */
						if (Transform * transform = ent->get<Transform>())
						{
							Funcs::Group("Transform", [&]()
							{
								Funcs::Field("Actions", [&](CString) 
								{
									if (ImGui::Selectable("Reset"))
									{
										transform->update(mat4::Identity());
									}
								});

								Funcs::Field("Position", [&](CString)
								{
									vec3 pos = transform->getPos();
									if (GUI::EditVec3f("##Position##Transform", pos, 0.01f))
									{
										(*transform)
											.translate(pos - transform->getPos())
											.rotate(0.0f, vec3::One)
											.scale(1.0f)
											;
									}
								});

								Funcs::Field("Rotation", [&](CString) 
								{
									quat rot = transform->getRot();
									if (GUI::EditQuat("##Rotation##Transform", rot, 0.01f))
									{
										(*transform)
											.translate(0.0f)
											.rotate(rot)
											.scale(1.0f)
											;
									}
								});
								
								Funcs::Field("Scale", [&](CString)
								{
									vec3 scl = transform->getScl();
									if (GUI::EditVec3f("##Scale##Transform", scl, 0.01f))
									{
										(*transform)
											.translate(0.0f)
											.rotate(0.0f, vec3::One)
											.scale(scl)
											;
									}
								});

								Funcs::Field("Matrix", [&](CString)
								{
									ImGui::NewLine();
									mat4 mat = transform->getMat();
									if (GUI::EditMat4f("##Matrix##Transform", mat, 0.01f))
									{
										transform->update(mat);
									}
									ImGui::NewLine();
								});

							});
						}
					}

					// File
					/* * * * * * * * * * * * * * * * * * * * */
					if (const String file = ML_Res.entities.getFile(name))
					{
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

					/* * * * * * * * * * * * * * * * * * * * */

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_fonts()
	{
		if (ML_Res.fonts.empty()) return;

		Funcs::Group(ML_Res.fonts.name().c_str(), [&]()
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
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_images()
	{
		if (ML_Res.images.empty()) return;

		Funcs::Group(ML_Res.images.name().c_str(), [&]()
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
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_lua()
	{
		if (ML_Res.lua.empty()) return;

		Funcs::Group(ML_Res.lua.name().c_str(), [&]()
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
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_materials()
	{
		if (ML_Res.materials.empty()) return;

		Funcs::Group(ML_Res.materials.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.materials)
			{
				Funcs::Group(pair.first.c_str(), [&](CString name, Material * mat)
				{
					// Name
					Funcs::Field("Name", [&](CString)
					{
						ImGui::Text("%s", name);
					});

					// Shader
					Funcs::Field("Shader", [&](CString)
					{
						List<String> keys = ML_Res.shaders.keys();
						int32_t index = ML_Res.shaders.getIndexOf(mat->shader());
						if (ImGui::Combo(
							"##Shader##Renderer",
							&index,
							ImGui_Helper::vector_getter,
							static_cast<void *>(&keys),
							(int32_t)(keys.size())))
						{
							if (const Shader * value = ML_Res.shaders.getByIndex(index))
							{
								mat->shader() = value;
							}
						}
					});

					// Uniforms
					Funcs::Group("Uniforms", [&]()
					{
						for (auto & pair : mat->uniforms())
						{
							switch (pair.second->type)
							{
							case uni_flt::ID:
								if (auto u = dynamic_cast<uni_flt *>(pair.second))
								{
									Funcs::Field(u->name.c_str(), [&](CString)
									{
										ImGui::DragFloat(String("##Float##Uni##" + pair.second->name).c_str(), &u->data, 0.1f);
									});
								}
								break;

							case uni_int::ID:
								if (auto u = dynamic_cast<uni_int *>(pair.second))
								{
									Funcs::Field(u->name.c_str(), [&](CString)
									{
										ImGui::DragInt(String("##Int##Uni##" + pair.second->name).c_str(), &u->data, 0.1f);
									});
								}
								break;

							case uni_vec2::ID:
								if (auto u = dynamic_cast<uni_vec2 *>(pair.second))
								{
									Funcs::Field(u->name.c_str(), [&](CString)
									{
										GUI::EditVec2f(String("##Vec2##Uni##" + pair.second->name).c_str(), u->data, 0.1f);
									});
								}
								break;

							case uni_vec3::ID:
								if (auto u = dynamic_cast<uni_vec3 *>(pair.second))
								{
									Funcs::Field(u->name.c_str(), [&](CString)
									{
										GUI::EditVec3f(String("##Vec3##Uni##" + pair.second->name).c_str(), u->data, 0.1f);
									});
								}
								break;

							case uni_vec4::ID:
								if (auto u = dynamic_cast<uni_vec4 *>(pair.second))
								{
									Funcs::Field(u->name.c_str(), [&](CString)
									{
										GUI::EditVec4f(String("##Vec4##Uni##" + pair.second->name).c_str(), u->data, 0.1f);
									});
								}
								break;

							case uni_col4::ID:
								if (auto u = dynamic_cast<uni_col4 *>(pair.second))
								{
									Funcs::Field(u->name.c_str(), [&](CString)
									{
										ImGui::ColorEdit4(String("##Color##Uni##" + pair.second->name).c_str(), &u->data[0]);
									});
								}
								break;

							case uni_mat3::ID:
								if (auto u = dynamic_cast<uni_mat3 *>(pair.second))
								{
									Funcs::Field(u->name.c_str(), [&](CString)
									{
										GUI::EditMat3f(String("##Mat3##Uni##" + pair.second->name).c_str(), u->data, 0.1f);
									});
								}
								break;

							case uni_mat4::ID:
								if (auto u = dynamic_cast<uni_mat4 *>(pair.second))
								{
									Funcs::Field(u->name.c_str(), [&](CString)
									{
										GUI::EditMat4f(String("##Mat4##Uni##" + pair.second->name).c_str(), u->data, 0.1f);
									});
								}
								break;

							case uni_tex_cp::ID:
								if (auto u = dynamic_cast<uni_tex_cp *>(pair.second))
								{
									Funcs::Field(pair.second->name.c_str(), [&](CString)
									{
										int32_t index = ML_Res.textures.getIndexOf(u->data);
										List<String> keys = ML_Res.textures.keys();
										if (ImGui::Combo(
											String("##Tex##Uni##" + pair.second->name).c_str(),
											&index,
											ImGui_Helper::vector_getter,
											static_cast<void *>(&keys),
											(int32_t)(keys.size())))
										{
											u->data = ML_Res.textures.getByIndex(index);
										}
									});
								}
								break;
							}
						}
					});

					// File
					if (const String file = ML_Res.materials.getFile(name))
					{
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_meshes()
	{
		if (ML_Res.meshes.empty()) return;

		Funcs::Group(ML_Res.meshes.name().c_str(), [&]()
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
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_models()
	{
		if (ML_Res.models.empty()) return;

		Funcs::Group(ML_Res.models.name().c_str(), [&]()
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
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_plugins()
	{
		if (ML_Res.plugins.empty()) return;

		Funcs::Group(ML_Res.plugins.name().c_str(), [&]()
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
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_scripts()
	{
		if (ML_Res.scripts.empty()) return;

		Funcs::Group(ML_Res.scripts.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.scripts)
			{
				Funcs::Group(pair.first.c_str(), [&](CString name, Script * scr)
				{
					Funcs::Field("Name", [&](CString)
					{
						ImGui::Text("%s", name);
					});
					Funcs::Field("Actions", [&](CString)
					{
						if (ImGui::Selectable("Build"))
						{
							scr->build({});
						}
						if (ImGui::Selectable("Run"))
						{
							scr->run();
						}
						if (ImGui::Selectable("Build & Run"))
						{
							scr->buildAndRun({});
						}
					});
					if (const String file = ML_Res.scripts.getFile(name))
					{
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_shaders()
	{
		if (ML_Res.shaders.empty()) return;

		Funcs::Group(ML_Res.shaders.name().c_str(), [&]()
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
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_skyboxes()
	{
		if (ML_Res.skyboxes.empty()) return;

		Funcs::Group(ML_Res.skyboxes.name().c_str(), [&]()
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
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_sounds()
	{
		if (ML_Res.sounds.empty()) return;

		Funcs::Group(ML_Res.sounds.name().c_str(), [&]()
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
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_sprites()
	{
		if (ML_Res.entities.empty()) return;

		Funcs::Group(ML_Res.sprites.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.sprites)
			{
				Funcs::Group(pair.first.c_str(), [&](CString name, Sprite * spr)
				{
					Funcs::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					Funcs::Field("Color", [&](CString label)
					{
						auto temp = spr->color();
						if (ImGui::ColorEdit4("##Color##Sprite", &temp[0]))
						{
							spr->setColor(temp);
						}
					});
					Funcs::Field("Origin", [&](CString label)
					{
						auto temp = spr->origin();
						if (GUI::EditVec2f("##Origin##Sprite", temp))
						{
							spr->setOrigin(temp);
						}
					});
					Funcs::Field("Position", [&](CString label)
					{
						auto temp = spr->position();
						if (GUI::EditVec2f("##Position##Sprite", temp))
						{
							spr->setPosition(temp);
						}
					});
					Funcs::Field("Rotation", [&](CString label)
					{
						auto temp = spr->rotation();
						if (ImGui::DragFloat("##Rotation##Sprite", &temp, 0.5f))
						{
							spr->setRotation(temp);
						}
					});
					Funcs::Field("Scale", [&](CString label)
					{
						auto temp = spr->scale();
						if (GUI::EditVec2f("##Scale##Sprite", temp))
						{
							spr->setScale(temp);
						}
					});
					Funcs::Field("Texture", [&](CString label)
					{
						int32_t index = ML_Res.textures.getIndexOf(spr->texture());
						List<String> keys = ML_Res.textures.keys();
						if (ImGui::Combo(
							"##Tex2D##Value",
							&index,
							ImGui_Helper::vector_getter,
							static_cast<void *>(&keys),
							(int32_t)(keys.size())))
						{
							if (const Texture * value = ML_Res.textures.getByIndex(index))
							{
								spr->setTexture(value);
							}
						}
					});
					if (const String file = ML_Res.sprites.getFile(name))
					{
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_textures()
	{
		if (ML_Res.textures.empty()) return;

		Funcs::Group(ML_Res.textures.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.textures)
			{
				Funcs::Group(pair.first.c_str(), [&](CString name, Texture * tex)
				{
					Funcs::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					Funcs::Field("Size", [&](CString label)
					{
						ImGui::Text("%u x %u", tex->width(), tex->height());
					});
					Funcs::Field("Real Size", [&](CString label)
					{
						ImGui::Text("%u x %u", tex->realWidth(), tex->realHeight());
					});
					Funcs::Field("Mipmapped", [&](CString label)
					{
						bool temp = tex->mipmapped();
						if (ImGui::Checkbox("##Texture##Mipmapped", &temp))
						{
							tex->setMipmapped(temp);
						}
					});
					Funcs::Field("Repeated", [&](CString label)
					{
						bool temp = tex->repeated();
						if (ImGui::Checkbox("##Texture##Repeated", &temp))
						{
							tex->setRepeated(temp);
						}
					});
					Funcs::Field("Smooth", [&](CString label)
					{
						bool temp = tex->smooth();
						if (ImGui::Checkbox("##Texture##Smooth", &temp))
						{
							tex->setSmooth(temp);
						}
					});

					if (const String file = ML_Res.textures.getFile(name))
					{
						Funcs::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Funcs::Field("Path", [&](CString)
						{
							const String fPath = ML_FS.getFilePath(file);
							if (ImGui::Selectable(fPath.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(fPath)));
							}
						});
					}

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	/* * * * * * * * * * * * * * * * * * * * */
}