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
	struct ResourceView::Layout
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Fun, class ... Args
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
			class Fun, class ... Args
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
			class Fun, class ... Args
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

		inline static int32_t EditUniform(const String & label, uni_base * value, bool show_constants)
		{
			int32_t flag = 0;

			auto toolbar_editable = [&]()
			{
				if (ImGui::Button(String("Delete" + label).c_str())) { flag = 1; }
			};

			auto toolbar_constant = [&]()
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.4f, 0.4f, 1.0f });
				ImGui::Text(" [const]");
				ImGui::PopStyleColor();
			};

			switch (value->type)
			{
				// Flt
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_flt::ID:
				if (auto u = dynamic_cast<uni_flt *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						ImGui::DragFloat(String(label + "##Float##Uni##" + value->name).c_str(), &u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_flt_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							ImGui::DragFloat(String(label + "##Float##Uni##" + value->name).c_str(), &temp, 0.1f);
						});
						break;
					}
				}

				// Int
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_int::ID:
				if (auto u = dynamic_cast<uni_int *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						ImGui::DragInt(String(label + "##Int##Uni##" + value->name).c_str(), &u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_int_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							ImGui::DragInt(String(label + "##Int##Uni##" + value->name).c_str(), &temp, 0.1f);
						});
						break;
					}
				}

				// Vec2
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_vec2::ID:
				if (auto u = dynamic_cast<uni_vec2 *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						GUI::EditVec2f(String(label + "##Vec2##Uni##" + value->name).c_str(), u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_vec2_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							GUI::EditVec2f(String(label + "##Vec2##Uni##" + value->name).c_str(), temp, 0.1f);
						});
						break;
					}
				}

				// Vec3
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_vec3::ID:
				if (auto u = dynamic_cast<uni_vec3 *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						GUI::EditVec3f(String(label + "##Vec3##Uni##" + value->name).c_str(), u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_vec3_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							GUI::EditVec3f(String(label + "##Vec3##Uni##" + value->name).c_str(), temp, 0.1f);
						});
						break;
					}
				}

				// Vec4
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_vec4::ID:
				if (auto u = dynamic_cast<uni_vec4 *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						GUI::EditVec4f(String(label + "##Vec4##Uni##" + value->name).c_str(), u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_vec4_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							GUI::EditVec4f(String(label + "##Vec4##Uni##" + value->name).c_str(), temp, 0.1f);
						});
						break;
					}
				}

				// Col4
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_col4::ID:
				if (auto u = dynamic_cast<uni_col4 *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						ImGui::ColorEdit4(String(label + "##Color##Uni##" + value->name).c_str(), &u->data[0]);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_col4_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							ImGui::ColorEdit4(String(label + "##Color##Uni##" + value->name).c_str(), &temp[0]);
						});
						break;
					}
				}

				// Mat3
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_mat3::ID:
				if (auto u = dynamic_cast<uni_mat3 *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						GUI::EditMat3f(String(label + "##Mat3##Uni##" + value->name).c_str(), u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_mat3_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							GUI::EditMat3f(String(label + "##Mat3##Uni##" + value->name).c_str(), temp, 0.1f);
						});
						break;
					}
				}

				// Mat4
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_mat4::ID:
				if (auto u = dynamic_cast<uni_mat4 *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						GUI::EditMat4f(String(label + "##Mat4##Uni##" + value->name).c_str(), u->data, 0.1f);
						toolbar_editable();
					});
					break;
				}
				else if (show_constants)
				{
					if (auto u = dynamic_cast<uni_mat4_cr *>(value))
					{
						toolbar_constant();
						Layout::Field(value->name.c_str(), [&](CString)
						{
							auto temp = u->data;
							GUI::EditMat4f(String(label + "##Mat4##Uni##" + value->name).c_str(), temp, 0.1f);
						});
						break;
					}
				}

				// Tex
				/* * * * * * * * * * * * * * * * * * * * */
			case uni_tex_cp::ID:
				if (auto u = dynamic_cast<uni_tex_cp *>(value))
				{
					Layout::Field(value->name.c_str(), [&](CString)
					{
						int32_t index = ML_Res.textures.getIndexOf(u->data);
						List<String> keys = ML_Res.textures.keys();
						if (ImGui::Combo(
							String(label + "##Tex##Uni##" + value->name).c_str(),
							&index,
							ImGui_Helper::vector_getter,
							static_cast<void *>(&keys),
							(int32_t)(keys.size())))
						{
							u->data = ML_Res.textures.getByIndex(index);
						}
						toolbar_editable();
					});

				}
				break;
			}

			ImGui::Separator();

			return flag;
		}

		inline static void NewUniform(Material * mat)
		{
			if (ImGui::Button("New Uniform..."))
			{
				ImGui::OpenPopup("New Uniform Editor");
			}
			if (ImGui::BeginPopupModal("New Uniform Editor", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				static const CString typeList[] =
				{
					"Float",
					"Int",
					"Vec2",
					"Vec3",
					"Vec4",
					"Col4",
					"Mat3",
					"Mat4",
					"Tex",
				};
				static int32_t type = 0;
				static char name[32] = "New_Uniform\0";

				auto closePopup = [&]()
				{
					type = 0;
					std::strcpy(name, "New_Uniform\0");
					ImGui::CloseCurrentPopup();
				};

				ImGui::Combo("Type", &type, typeList, IM_ARRAYSIZE(typeList));
				ImGui::InputText("Name", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue);

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				if (ImGui::Button("Submit"))
				{
					if (String(name) && !mat->find_any(name))
					{
						uni_base * u = NULL;
						switch (type)
						{
						case uni_base::Flt: u = new uni_flt(name, 0);	break;
						case uni_base::Int: u = new uni_int(name, 0);	break;
						case uni_base::Vec2: u = new uni_vec2(name, 0);	break;
						case uni_base::Vec3: u = new uni_vec3(name, 0);	break;
						case uni_base::Vec4: u = new uni_vec4(name, 0);	break;
						case uni_base::Col4: u = new uni_col4(name, 0);	break;
						case uni_base::Mat3: u = new uni_mat3(name, 0);	break;
						case uni_base::Mat4: u = new uni_mat4(name, 0);	break;
						case uni_base::Tex: u = new uni_tex_cp(name, 0);	break;
						}
						if (u && (u = mat->uniforms().insert({ name, u }).first->second))
						{
							closePopup();
						}
					}
					else
					{
						Debug::logError("A uniform with that name already exists!");
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					closePopup();
				}

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				ImGui::EndPopup();
			}
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
					if (ImGui::MenuItem("Effect")) { /**/ }
					if (ImGui::MenuItem("Entity")) { /**/ }
					if (ImGui::MenuItem("Font")) { /**/ }
					if (ImGui::MenuItem("Image")) { /**/ }
					if (ImGui::MenuItem("Lua")) { /**/ }
					if (ImGui::MenuItem("Material")) { /**/ }
					if (ImGui::MenuItem("Mesh")) { /**/ }
					if (ImGui::MenuItem("Model")) { /**/ }
					if (ImGui::MenuItem("Plugin")) { /**/ }
					if (ImGui::MenuItem("Script")) { /**/ }
					if (ImGui::MenuItem("Shader")) { /**/ }
					if (ImGui::MenuItem("Skybox")) { /**/ }
					if (ImGui::MenuItem("Sound")) { /**/ }
					if (ImGui::MenuItem("Sprite")) { /**/ }
					if (ImGui::MenuItem("Texture")) { /**/ }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			Layout::Columns([&]()
			{
				draw_effects();
				draw_entities();
				draw_fonts();
				draw_images();
				draw_lua();
				draw_meshes();
				draw_materials();
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

	void ResourceView::draw_effects()
	{
		if (ML_Res.effects.empty()) return;

		Layout::Group(ML_Res.effects.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.effects)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Effect * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = ML_Res.effects.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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

		Layout::Group(ML_Res.entities.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.entities)
			{
				ImGui::PushID(pair.first.c_str());
				
				Layout::Group(pair.first.c_str(), [&](CString name, Entity * ent)
				{
					// Name
					/* * * * * * * * * * * * * * * * * * * * */
					Layout::Field("Name", [&](CString)
					{
						ImGui::Text("%s", name);
					});

					// Components
					/* * * * * * * * * * * * * * * * * * * * */
					{
						// Transform
						/* * * * * * * * * * * * * * * * * * * * */
						if (Transform * transform = ent->get<Transform>())
						{
							Layout::Group("Transform", [&]()
							{
								Layout::Field("Actions", [&](CString)
								{
									if (ImGui::Selectable("Reset"))
									{
										transform->update(mat4::Identity());
									}
								});

								Layout::Field("Position", [&](CString)
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

								Layout::Field("Rotation", [&](CString)
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

								Layout::Field("Euler Angles", [&](CString)
								{
									quat rot = transform->getRot();

									vec3 euler = rot.eulerAngles();
									GUI::EditVec3f("##EulerAngles##Transform", euler);

								});

								Layout::Field("Scale", [&](CString)
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

								Layout::Field("Matrix", [&](CString)
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

						// Camera
						/* * * * * * * * * * * * * * * * * * * * */
						if (Camera * camera = ent->get<Camera>())
						{
							Layout::Group("Camera", [&]()
							{
								Layout::Field("Color", [&](CString)
								{
									ImGui::ColorEdit4("##Color##Camera", &camera->color[0]);
								});
								Layout::Field("FOV", [&](CString)
								{
									ImGui::DragFloat("##FOV##Camera", &camera->fov, 0.5f, 10.f, 100.f);
								});
								Layout::Group("Perspective", [&]()
								{
									Layout::Field("Near", [&](CString)
									{
										ImGui::DragFloat("##pNear##Camera", &camera->pNear, 0.5f, 10.f, camera->pFar - 1.f);
									});
									Layout::Field("Far", [&](CString)
									{
										ImGui::DragFloat("##pFar##Camera", &camera->pFar, 0.5f, camera->pNear + 1.f, 1000.0f);
									});
									Layout::Field("Matrix", [&](CString)
									{
										mat4 mat = camera->getPerspMatrix();
										GUI::EditMat4f("##O##Matrix##Camera", mat);
									});
								});
								Layout::Group("Orthographic", [&]()
								{
									Layout::Field("Near", [&](CString)
									{
										ImGui::DragFloat("##oNear##Camera", &camera->oNear, 0.5f, 10.f, camera->oFar - 1.f);
									});
									Layout::Field("Far", [&](CString)
									{
										ImGui::DragFloat("##oFar##Camera", &camera->oFar, 0.5f, camera->oNear + 1.f, 100.f);
									});
									Layout::Field("Matrix", [&](CString)
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
							Layout::Group("Light", [&]()
							{
								Layout::Field("Color", [&](CString)
								{
									ImGui::ColorEdit4("##Color##Light", &light->color[0]);
								});
							});
						}

						// Renderer
						/* * * * * * * * * * * * * * * * * * * * */
						if (Renderer * renderer = ent->get<Renderer>())
						{
							Layout::Group("Renderer", [&]()
							{
								// Model
								Layout::Field("Model", [&](CString)
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
								for (auto & pair : renderer->states())
								{
									switch (pair.first)
									{
									case GL::AlphaTest:
										Layout::Field("Alpha Test", [&](CString)
										{
											ImGui::Checkbox("##AlphaTest##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::Blend:
										Layout::Field("Blend", [&](CString)
										{
											ImGui::Checkbox("##Blend##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::CullFace:
										Layout::Field("Cull Face", [&](CString)
										{
											ImGui::Checkbox("##CullFace##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::DepthTest:
										Layout::Field("Depth Test", [&](CString)
										{
											ImGui::Checkbox("##DepthTest##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::Multisample:
										Layout::Field("Multisample", [&](CString)
										{
											ImGui::Checkbox("##Multisample##States", (bool *)(pair.second.ptr()));
										});
										break;
									case GL::Texture2D:
										Layout::Field("Texture2D", [&](CString)
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
							Layout::Group("Rigidbody", [&]()
							{
								Layout::Field("Enabled", [&](CString)
								{
									ImGui::Checkbox("##Enabled##Rigidbody", &rb->enabled);
								});

								Layout::Field("Index", [&](CString)
								{
									ImGui::Text("%i", rb->index());
								});

								Layout::Group("Motion Controller", [&]()
								{
									Layout::Field("Actions", [&](CString)
									{
										if (ImGui::Selectable("Reset##Particle##Rigidbody"))
										{
											rb->particle()->reset();
										}
									});
									Layout::Field("Mass",					[&](CString) {	ImGui::DragFloat("##Mass##Particle##Rigidbody",							&rb->particle()->mass);});
									Layout::Field("Mass Inv",				[&](CString) {	ImGui::DragFloat("##MassInv##Particle##Rigidbody",						&rb->particle()->massInv);});
									Layout::Field("Position",				[&](CString) {	GUI::EditVec3f("##Position##Particle##Rigidbody",						rb->particle()->pos);});
									Layout::Field("Velocity",				[&](CString) {	GUI::EditVec3f("##Velocity##Particle##Rigidbody",						rb->particle()->vel);});
									Layout::Field("Acceleration",			[&](CString) {	GUI::EditVec3f("##Acceleration##Particle##Rigidbody",					rb->particle()->acc);});
									Layout::Field("Force",					[&](CString) {	GUI::EditVec3f("##Force##Particle##Rigidbody",							rb->particle()->force);});
									Layout::Field("Momentum",				[&](CString) {	GUI::EditVec3f("##Momentum##Particle##Rigidbody",						rb->particle()->momentum);});
									Layout::Field("Rotation",				[&](CString) {	GUI::EditQuat ("##Rotation##Particle##Rigidbody",						rb->particle()->rotation);});
									Layout::Field("Angular Velocity",		[&](CString) {	GUI::EditVec3f("##Angular Velocity##Particle##Rigidbody",				rb->particle()->angularVel);});
									Layout::Field("Angular Acceleration",	[&](CString) {	GUI::EditVec3f("##Angular Acceleration##Particle##Rigidbody",			rb->particle()->angularAcc);});
									Layout::Field("Angular Momentum",		[&](CString) {	GUI::EditVec3f("##Angular Momentum##Particle##Rigidbody",				rb->particle()->angularMomentum);});
									Layout::Field("Torque",					[&](CString) {	GUI::EditVec3f("##Torque##Particle##Rigidbody",							rb->particle()->torque);});
									Layout::Field("Inirtia Tensor",			[&](CString) {	GUI::EditMat3f("##Inirtia Tensor##Particle##Rigidbody",					rb->particle()->inertiaTensor);});
									Layout::Field("IT Inverse",				[&](CString) {	GUI::EditMat3f("##Inirtia Tensor Inverse##Particle##Rigidbody",			rb->particle()->inertiaTensorInv);});
									Layout::Field("IT World",				[&](CString) {	GUI::EditMat3f("##Inirtia Tensor World##Particle##Rigidbody",			rb->particle()->inertiaTensor_world);});
									Layout::Field("IT World Inverse",		[&](CString) {	GUI::EditMat3f("##Inirtia Tensor World Inverse##Particle##Rigidbody",	rb->particle()->inertiaTensorInv_world);});
									Layout::Field("Center Mass",			[&](CString) {	GUI::EditVec3f("##Center Mass##Particle##Rigidbody",					rb->particle()->centerMass);});
									Layout::Field("Center Mass World",		[&](CString) {	GUI::EditVec3f("##Center Mass World##Particle##Rigidbody",				rb->particle()->centerMass_world);});

								});

								Layout::Field("Collider", [&](CString)
								{
									ImGui::Text("%s", rb->collider() ? "OK" : "NULL");
								});

								Layout::Field("Transform", [&](CString)
								{
									ImGui::Text("%s", rb->transform() ? "OK" : "NULL");
								});
							});
						}
					}

					// File
					/* * * * * * * * * * * * * * * * * * * * */
					if (const String file = ML_Res.entities.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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
			
				ImGui::PopID();
}
		});
	}

	void ResourceView::draw_fonts()
	{
		if (ML_Res.fonts.empty()) return;

		Layout::Group(ML_Res.fonts.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.fonts)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Font * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Selectable(name);
					});
					Layout::Field("Family", [&](CString label)
					{
						ImGui::Selectable(e->getInfo().family.c_str());
					});
					if (const String file = ML_Res.fonts.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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

		Layout::Group(ML_Res.images.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.images)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Image * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					Layout::Field("Size", [&](CString label)
					{
						ImGui::Text("%u x %u", e->width(), e->height());
					});
					if (const String file = ML_Res.images.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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

		Layout::Group(ML_Res.lua.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.lua)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const LuaScript * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = ML_Res.lua.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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

		Layout::Group(ML_Res.materials.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.materials)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, Material * mat)
				{
					// Name
					Layout::Field("Name", [&](CString)
					{
						ImGui::Text("%s", name);
					});

					// Shader
					Layout::Field("Shader", [&](CString)
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
					Layout::Group("Uniforms", [&]()
					{
						static bool show_constants = true;

						ImGui::Checkbox("Show Constants", &show_constants);

						Layout::NewUniform(mat);

						if (!mat->uniforms().empty())
						{
							ImGui::Separator();
						}

						std::vector<Material::UniformMap::iterator> toRemove;

						for (auto it = mat->uniforms().begin(); it != mat->uniforms().end(); it++)
						{
							const String label("##" + String(name) + "##Uniform##" + it->first);

							const int32_t flag = Layout::EditUniform(label, it->second, show_constants);

							if (flag == 1)
							{
								toRemove.push_back(it);
							}
						}

						for (auto it : toRemove)
						{
							uni_base * u = it->second;
							mat->uniforms().erase(it);
							delete u;
						}

					});

				}, pair.first.c_str(), pair.second);
			}
		});
	}

	void ResourceView::draw_meshes()
	{
		if (ML_Res.meshes.empty()) return;

		Layout::Group(ML_Res.meshes.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.meshes)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Mesh * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = ML_Res.meshes.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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

		Layout::Group(ML_Res.models.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.models)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Model * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = ML_Res.models.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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

		Layout::Group(ML_Res.plugins.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.plugins)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Plugin * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = ML_Res.plugins.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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

		Layout::Group(ML_Res.scripts.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.scripts)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, Script * scr)
				{
					Layout::Field("Name", [&](CString)
					{
						ImGui::Text("%s", name);
					});
					Layout::Field("Actions", [&](CString)
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
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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

		Layout::Group(ML_Res.shaders.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.shaders)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Shader * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = ML_Res.shaders.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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

		Layout::Group(ML_Res.skyboxes.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.skyboxes)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Skybox * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = ML_Res.skyboxes.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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

		Layout::Group(ML_Res.sounds.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.sounds)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, const Sound * e)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					if (const String file = ML_Res.sounds.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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

		Layout::Group(ML_Res.sprites.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.sprites)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, Sprite * spr)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					Layout::Field("Color", [&](CString label)
					{
						auto temp = spr->color();
						if (ImGui::ColorEdit4("##Color##Sprite", &temp[0]))
						{
							spr->setColor(temp);
						}
					});
					Layout::Field("Origin", [&](CString label)
					{
						auto temp = spr->origin();
						if (GUI::EditVec2f("##Origin##Sprite", temp))
						{
							spr->setOrigin(temp);
						}
					});
					Layout::Field("Position", [&](CString label)
					{
						auto temp = spr->position();
						if (GUI::EditVec2f("##Position##Sprite", temp))
						{
							spr->setPosition(temp);
						}
					});
					Layout::Field("Rotation", [&](CString label)
					{
						auto temp = spr->rotation();
						if (ImGui::DragFloat("##Rotation##Sprite", &temp, 0.5f))
						{
							spr->setRotation(temp);
						}
					});
					Layout::Field("Scale", [&](CString label)
					{
						auto temp = spr->scale();
						if (GUI::EditVec2f("##Scale##Sprite", temp))
						{
							spr->setScale(temp);
						}
					});
					Layout::Field("Texture", [&](CString label)
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
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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

		Layout::Group(ML_Res.textures.name().c_str(), [&]()
		{
			for (auto & pair : ML_Res.textures)
			{
				Layout::Group(pair.first.c_str(), [&](CString name, Texture * tex)
				{
					Layout::Field("Name", [&](CString label)
					{
						ImGui::Text("%s", name);
					});
					Layout::Field("Size", [&](CString label)
					{
						ImGui::Text("%u x %u", tex->width(), tex->height());
					});
					Layout::Field("Real Size", [&](CString label)
					{
						ImGui::Text("%u x %u", tex->realWidth(), tex->realHeight());
					});
					Layout::Field("Mipmapped", [&](CString label)
					{
						bool temp = tex->mipmapped();
						if (ImGui::Checkbox("##Texture##Mipmapped", &temp))
						{
							tex->setMipmapped(temp);
						}
					});
					Layout::Field("Repeated", [&](CString label)
					{
						bool temp = tex->repeated();
						if (ImGui::Checkbox("##Texture##Repeated", &temp))
						{
							tex->setRepeated(temp);
						}
					});
					Layout::Field("Smooth", [&](CString label)
					{
						bool temp = tex->smooth();
						if (ImGui::Checkbox("##Texture##Smooth", &temp))
						{
							tex->setSmooth(temp);
						}
					});

					if (const String file = ML_Res.textures.getFile(name))
					{
						Layout::Field("File", [&](CString)
						{
							const String fName = ML_FS.getFileName(file);
							if (ImGui::Selectable(fName.c_str()))
							{
								ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_FS.getPathTo(file)));
							}
						});
						Layout::Field("Path", [&](CString)
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