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
#include <MemeGraphics/Camera.hpp>

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
		Funcs::Group(ML_Res.effects.name().c_str(), [&]()
		{
			if (ML_Res.effects.empty())
			{
				return ImGui::Text("None");
			}

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
		Funcs::Group(ML_Res.entities.name().c_str(), [&]()
		{
			if (ML_Res.entities.empty())
			{
				return ImGui::Text("None");
			}

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
								Funcs::Field("Position", [&](CString)
								{
									GUI::EditVec3f("##Position##Camera", camera->position);
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

								// Shader
								Funcs::Field("Shader", [&](CString)
								{
									List<String> keys = ML_Res.shaders.keys();
									int32_t index = ML_Res.shaders.getIndexOf(renderer->material().shader());
									if (ImGui::Combo(
										"##Shader##Renderer",
										&index,
										ImGui_Helper::vector_getter,
										static_cast<void *>(&keys),
										(int32_t)(keys.size())))
									{
										if (const Shader * value = ML_Res.shaders.getByIndex(index))
										{
											renderer->material().shader() = value;
										}
									}
								});
								
								// States
								Funcs::Group("States", [&]()
								{
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

								// Uniforms
								Funcs::Group("Uniforms", [&]()
								{
									for (auto & pair : renderer->material().uniforms())
									{
										ImGui::PushID(pair.second.name.c_str());
										Funcs::Field(pair.first.c_str(), [&](CString, Uniform * uni)
										{
											switch (uni->type)
											{
											case Uniform::Tex2D:
											{
												int32_t index = ML_Res.textures.getIndexOf(uni->get_pointer<Texture>());
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
														uni->data = value;
													}
												}
											}
											break;
											case Uniform::Int:
											{
												int32_t temp = uni->get_value<int32_t>();
												if (ImGui::DragInt("##Int##Value", &temp, 0.1f)) {}
											}
											break;
											case Uniform::Float:
											{
												float temp = uni->get_value<float>();
												if (ImGui::DragFloat("##Float##Value", &temp, 0.1f)) {}
											}
											break;
											case Uniform::Vec2:
											{
												vec2f temp = uni->get_value<vec2f>();
												if (GUI::EditVec2f("##Vec2##Value", temp, 0.1f)) {}
											}
											break;
											case Uniform::Vec3:
											{
												vec3f temp = uni->get_value<vec3f>();
												if (GUI::EditVec3f("##Vec3##Value", temp, 0.1f)) {}
											}
											break;
											case Uniform::Vec4:
											{
												vec4f temp = uni->get_value<vec4f>();
												if (GUI::EditVec4f("##Vec4##Value", temp, 0.1f)) {}
											}
											break;
											case Uniform::Mat3:
											{
												mat3f temp = uni->get_value<mat3f>();
												if (GUI::EditMat3f("##Mat3##Value", temp, 0.1f)) {}
											}
											break;
											case Uniform::Mat4:
											{
												mat4f temp = uni->get_value<mat4f>();
												if (GUI::EditMat4f("##Mat4##Value", temp, 0.1f)) {}
											}
											break;
											}

										}, &pair.second);
										ImGui::PopID();
									}
								});
							});
						}

						// Rigidbody
						/* * * * * * * * * * * * * * * * * * * * */
						if (Rigidbody * rigidbody = ent->get<Rigidbody>())
						{
							Funcs::Group("Rigidbody", [&]()
							{
								ImGui::Text("OK");
							});
						}

						// Transform
						/* * * * * * * * * * * * * * * * * * * * */
						if (Transform * transform = ent->get<Transform>())
						{
							Funcs::Group("Transform", [&]()
							{
								Funcs::Group("Decompose", [&]()
								{
									vec3f scale;
									quat  orient;
									vec3f trans;
									vec3f skew;
									vec4f persp;

									if (transform->decompose(scale, orient, trans, skew, persp))
									{
										Funcs::Field("Scale", [&](CString label)
										{
											GUI::EditVec3f("##Scale##Transform##Decompose", scale);
										});

										Funcs::Field("Orientation", [&](CString label)
										{
											GUI::EditVec4f("##Orientation##Transform##Decompose", orient);
										});

										Funcs::Field("Translation", [&](CString label)
										{
											GUI::EditVec3f("##Translation##Transform##Decompose", trans);
										});

										Funcs::Field("Skew", [&](CString label)
										{
											GUI::EditVec3f("##Skew##Transform##Decompose", skew);
										});

										Funcs::Field("Perspective", [&](CString label)
										{
											GUI::EditVec4f("##Perspective##Transform##Decompose", persp);
										});

										ImGui::NewLine();
									}
								});

								Funcs::Group("Matrix", [&]()
								{
									Funcs::Field("Value", [&](CString)
									{
										mat4f matrix = transform->matrix();
										if (GUI::EditMat4f("##Matrix##Transform", matrix, 0.1f))
										{
											transform->matrix() = matrix;
										}
										ImGui::NewLine();
									});
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
		Funcs::Group(ML_Res.fonts.name().c_str(), [&]()
		{
			if (ML_Res.fonts.empty())
			{
				return ImGui::Text("None");
			}

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
		Funcs::Group(ML_Res.images.name().c_str(), [&]()
		{
			if (ML_Res.images.empty())
			{
				return ImGui::Text("None");
			}

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
		Funcs::Group(ML_Res.lua.name().c_str(), [&]()
		{
			if (ML_Res.lua.empty())
			{
				return ImGui::Text("None");
			}

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
		Funcs::Group(ML_Res.materials.name().c_str(), [&]()
		{
			if (ML_Res.materials.empty())
			{
				return ImGui::Text("None");
			}

			for (auto & pair : ML_Res.materials)
			{
				Funcs::Group(pair.first.c_str(), [&](CString name, const Material * mat)
				{
					Funcs::Field("Name", [&](CString)
					{
						ImGui::Text("%s", name);
					});

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
		Funcs::Group(ML_Res.meshes.name().c_str(), [&]()
		{
			if (ML_Res.meshes.empty())
			{
				return ImGui::Text("None");
			}

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
		Funcs::Group(ML_Res.models.name().c_str(), [&]()
		{
			if (ML_Res.models.empty())
			{
				return ImGui::Text("None");
			}

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
		Funcs::Group(ML_Res.plugins.name().c_str(), [&]()
		{
			if (ML_Res.plugins.empty())
			{
				return ImGui::Text("None");
			}

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
		Funcs::Group(ML_Res.scripts.name().c_str(), [&]()
		{
			if (ML_Res.scripts.empty())
			{
				return ImGui::Text("None");
			}

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
		Funcs::Group(ML_Res.shaders.name().c_str(), [&]()
		{
			if (ML_Res.shaders.empty())
			{
				return ImGui::Text("None");
			}

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
		Funcs::Group(ML_Res.skyboxes.name().c_str(), [&]()
		{
			if (ML_Res.skyboxes.empty())
			{
				return ImGui::Text("None");
			}

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
		Funcs::Group(ML_Res.sounds.name().c_str(), [&]()
		{
			if (ML_Res.sounds.empty())
			{
				return ImGui::Text("None");
			}

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
		Funcs::Group(ML_Res.sprites.name().c_str(), [&]()
		{
			if (ML_Res.sprites.empty())
			{
				return ImGui::Text("None");
			}

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
		Funcs::Group(ML_Res.textures.name().c_str(), [&]()
		{
			if (ML_Res.textures.empty())
			{
				return ImGui::Text("None");
			}

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