/* * * * * * * * * * * * * * * * * * * * */

#include "Sandbox.hpp"
#include "Settings.hpp"
#include "DemoPhysics.hpp"

#include <MemeAudio/Audio.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/Dispatcher.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeCore/FileSystem.hpp> 
#include <MemeCore/Random.hpp>
#include <MemeCore/OS.hpp>
#include <MemeCore/Time.hpp>
#include <MemeGraphics/Camera.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeGraphics/Renderer.hpp>
#include <MemeGraphics/ShaderAPI.hpp>
#include <MemeGraphics/GraphicsEvents.hpp>
#include <MemeGraphics/Canvas.hpp>
#include <MemeGraphics/Light.hpp>
#include <MemeGraphics/Uni.hpp>
#include <MemeEditor/Editor.hpp>
#include <MemeEditor/EditorCommands.hpp>
#include <MemeEditor/EditorEvents.hpp>
#include <MemeEditor/GUI.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEditor/ImGui_Style.hpp>
#include <MemeEditor/StyleLoader.hpp>
#include <MemeEditor/AnyVar.hpp>
#include <MemeEngine/Engine.hpp>
#include <MemeEngine/Entity.hpp>
#include <MemeEngine/EngineCommands.hpp>
#include <MemeEngine/Resources.hpp>
#include <MemeEngine/Preferences.hpp>
#include <MemeNet/NetClient.hpp>
#include <MemeNet/NetServer.hpp>
#include <MemePhysics/Physics.hpp>
#include <MemePhysics/Force.hpp>
#include <MemePhysics/Rigidbody.hpp>
#include <MemePhysics/BoxCollider.hpp>
#include <MemePhysics/SphereCollider.hpp>
#include <MemePhysics/Particle.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeWindow/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_CAMERA	ML_Res.entities.get("camera")
#define ML_LIGHT	ML_Res.entities.get("light")

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Sandbox::Sandbox() 
		: EditorApplication() 
	{
	}

	Sandbox::~Sandbox() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	void Sandbox::onEvent(const ml::IEvent * value)
	{
		// Handle base events
		ml::EditorApplication::onEvent(value);

		switch (*value)
		{
			// Keyboard Input
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::WindowEvent::EV_Key:
			if (const auto * ev = value->as<ml::KeyEvent>())
			{
				/* * * * * * * * * * * * * * * * * * * * */

				// Close (Escape)
				if (ev->getKeyDown(ml::KeyCode::Escape) && SETTINGS.escapeIsExit)
				{
					this->close();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				// Show Terminal (Ctrl+Alt+T)
				if (ev->getKeyDown(ml::KeyCode::T) && (ev->mod_ctrl && ev->mod_alt))
					ML_Editor.show_terminal = true;

				// Show Browser (Ctrl+Alt+E)
				if (ev->getKeyDown(ml::KeyCode::E) && (ev->mod_ctrl))
					ML_Editor.show_browser = true;

				// Show Builder (Ctrl+Alt+B)
				if (ev->getKeyDown(ml::KeyCode::B) && (ev->mod_ctrl && ev->mod_alt))
					ML_Editor.show_builder = true;

				// Show Scene (Ctrl+Alt+S)
				if (ev->getKeyDown(ml::KeyCode::S) && (ev->mod_ctrl && ev->mod_alt))
					ML_Editor.show_sceneView = true;

				// Show Inspector (Ctrl+Alt+I)
				if (ev->getKeyDown(ml::KeyCode::I) && (ev->mod_ctrl && ev->mod_alt))
					ML_Editor.show_inspector = true;

				// Show ImGui Demo (Ctrl+H)
				if (ev->getKeyDown(ml::KeyCode::H) && (ev->mod_ctrl))
					ML_Editor.show_imgui_demo = true;

				/* * * * * * * * * * * * * * * * * * * * */
			}
			break;
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Sandbox::onEnter(const ml::EnterEvent * ev)
	{
		// Initialize Miscellaneous
		/* * * * * * * * * * * * * * * * * * * * */
		{
			// Seed Random
			ML_Random.seed();

			// Start Master Timer
			ML_Time.start();

			// Setup Std Out
			if (SETTINGS.redirStdOut && !(m_rdbuf = ml::cout.rdbuf(m_rdstr.rdbuf())))
			{
				return ml::Debug::fatal("Failed Redirecting Std Output Handle");
			}
		}

		// Initialize Interpreter
		/* * * * * * * * * * * * * * * * * * * * */
		{
			// Set Parser Flags
			ML_Parser.showToks(SETTINGS.flagShowToks);
			ML_Parser.showTree(SETTINGS.flagShowTree);
			ML_Parser.showItoP(SETTINGS.flagShowItoP);

			// Install Commands
			ml::EngineCommands::install();
			ml::EditorCommands::install();

			// Run Boot Script
			ml::Script scr;
			if (scr.loadFromFile(ML_FS.getPathTo(SETTINGS.bootScript)))
			{
				if (!(scr.buildAndRun(ml::Args(ev->argc, ev->argv))))
				{
					ml::Debug::logError("Failed Running \'{0}\'", SETTINGS.bootScript);
				}
			}
		}

		// Initialize Window
		/* * * * * * * * * * * * * * * * * * * * */
		if (this->create(
			SETTINGS.title,
			ml::VideoMode({ SETTINGS.width, SETTINGS.height }, SETTINGS.bitsPerPixel),
			ml::Window::Style::Default, 
			ml::ContextSettings 
			{
				SETTINGS.majorVersion,
				SETTINGS.minorVersion,
				SETTINGS.profile,
				SETTINGS.depthBits,
				SETTINGS.stencilBits,
				SETTINGS.multisample,
				SETTINGS.srgbCapable
			}
		))
		{
			this->maximize();
			this->seCursorMode(ml::Cursor::Normal);
			this->setPosition((ml::VideoMode::desktop().size - this->getSize()) / 2);
			this->setViewport(ml::vec2i::Zero, this->getFrameSize());
		}
		else
		{
			return ml::Debug::fatal("Failed Initializing Window");
		}

		// Initialize ImGui
		/* * * * * * * * * * * * * * * * * * * * */
		if (IMGUI_CHECKVERSION())
		{
			ImGui::CreateContext();

			static ml::StyleLoader loader;
			if (loader.loadFromFile(ML_FS.getPathTo("../../../assets/styles/style4.txt")))
			{
				// TODO
			}

			ImGui::StyleHelper::Style4();

			if ((SETTINGS.imguiFontFile) && 
				(SETTINGS.imguiFontSize > 0.f))
			{
				ImGui::GetIO().Fonts->AddFontFromFileTTF(
					SETTINGS.imguiFontFile.c_str(),
					SETTINGS.imguiFontSize
				);
			}
			
			if (!ImGui_ML_Init("#version 410", this, true, SETTINGS.imguiINI.c_str()))
			{
				return ml::Debug::fatal("Failed Initializing ImGui");
			}
		}

		// Initialize Audio
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ML_AL.init())
		{
			return ml::Debug::fatal("Failed Initializing OpenAL");
		}

		// Initialize Network
		/* * * * * * * * * * * * * * * * * * * * */
		if (SETTINGS.isServer)
		{
			// Start Server
			ml::Debug::log("Starting Server...");
			if (ML_NetServer.setup())
			{
				if (ML_NetServer.start({ ML_LOCALHOST, ML_DEFAULT_PORT }, ML_MAX_CLIENTS))
				{
					ml::Debug::log("Server Started: {0}", ML_NetServer.getMyAddress());
				}
			}
		}
		else if (SETTINGS.isClient)
		{
			// Start Client
			ml::Debug::log("Starting Client...");
			if (ML_NetClient.setup())
			{
				if (ML_NetClient.connect({ ML_LOCALHOST, ML_DEFAULT_PORT }))
				{
					ml::Debug::log("Client Connected: {0}", ML_NetClient.getMyAddress());
				}
			}
		}
	}

	void Sandbox::onLoad(const ml::LoadEvent * ev)
	{
		// Create Canvas
		/* * * * * * * * * * * * * * * * * * * * */
		m_canvas.create();

		// Load Default Meshes
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Res.meshes.load("default_triangle")->loadFromMemory(
			ml::Shapes::Triangle::Vertices,
			ml::Shapes::Triangle::Indices
		);
		ML_Res.meshes.load("default_quad")->loadFromMemory(
			ml::Shapes::Quad::Vertices,
			ml::Shapes::Quad::Indices
		);
		ML_Res.meshes.load("default_cube")->loadFromMemory(
			ml::Shapes::Cube::Vertices,
			ml::Shapes::Cube::Indices
		);
		ML_Res.meshes.load("default_skybox")->loadFromMemory(
			ml::Shapes::Sky::Vertices
		);

		// Load Default Models
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Res.models.load("default_triangle")->loadFromMemory(
			*ML_Res.meshes.get("default_triangle")
		);
		ML_Res.models.load("default_quad")->loadFromMemory(
			*ML_Res.meshes.get("default_quad")
		);
		ML_Res.models.load("default_cube")->loadFromMemory(
			*ML_Res.meshes.get("default_cube")
		);
		ML_Res.models.load("default_skybox")->loadFromMemory(
			*ML_Res.meshes.get("default_skybox")
		);

		// Load Manifest
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ML_Res.loadFromFile(ML_FS.getPathTo(SETTINGS.manifest)))
		{
			ml::Debug::logError("Failed Loading Manifest");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Sandbox::onStart(const ml::StartEvent * ev)
	{
		// Set Icon
		/* * * * * * * * * * * * * * * * * * * * */
		if (const ml::Image * icon = ML_Res.images.get("icon"))
		{
			const ml::Image temp = ml::Image(*icon).flipVertically();

			this->setIcons({ temp });
		}

		// Setup Plugins
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ML_Res.plugins.empty())
		{
			if (ml::Plugin * plugin = ML_Res.plugins.get("TestPlugin"))
			{
				if (void * msg = plugin->lib().callFunction<void *>(
					ML_str(ML_Plugin_Test), "TEST"
					))
				{
					ml::Debug::log((ml::CString)(msg));
				}
				else
				{
					ml::Debug::logError("Plugin Callback Failed");
				}
			}
		}

		// Setup Sprites
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Sprite * spr = ML_Res.sprites.get("neutrino"))
		{
			spr->setPosition(ml::vec2(0.95f, 0.925f) * this->getSize())
				.setScale	(0.5f)
				.setRotation(0.0f)
				.setOrigin	(0.5f)
				.setColor	(ml::Color::White);
		}

		// Setup Entities
		/* * * * * * * * * * * * * * * * * * * * */
		{
			// Camera
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("camera"))
			{
				ml::Camera * camera = ent->add<ml::Camera>(
					SETTINGS.fieldOfView,
					SETTINGS.perspNear,
					SETTINGS.perspFar,
					SETTINGS.orthoNear,
					SETTINGS.orthoFar
				);
				camera->color = { ml::vec3 { 0.198f }, 1.0f };
				camera->position = { 0.0f, 1.0f, 10.0f };
				camera->forward(ml::vec3::Back);

				ml::Transform * transform = ent->add<ml::Transform>();
				transform->lookAt(camera->position, camera->forward());
			}

			// Light
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("light"))
			{
				ml::Transform * transform = ent->add<ml::Transform>(
					ml::vec3 { 0.0f, 1.0f, 30.0f }, // position
					ml::vec3 { 1.0f }, // scale
					ml::quat { } // rotation
				);

				ml::Light * light = ent->add<ml::Light>(
					ml::Color::LightYellow
				);

				const ml::Material * material = ML_Res.materials.load_forward(
					"mat_light",
					ML_Res.shaders.get("solid"),
					ml::List<ml::uni_base *>({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		ML_CAMERA->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		ML_CAMERA->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_col4_cr	(ML_FRAG_MAIN_COL,	light->color),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ML_Res.models.get("sphere8x6"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, 1 } },
					})
				);
			}

			// Borg
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("borg"))
			{
				ml::Transform * transform = ent->add<ml::Transform>(
					ml::vec3 { 5.0f, 0.0f, 0.0f }, // position
					ml::vec3 { 1.0f }, // scale
					ml::quat { } // rotation
				);

				ml::BoxCollider * collider = ent->add<ml::BoxCollider>(
					transform->getScl() // size
				);

				ml::Particle * particle = ent->add<ml::Particle>(
					transform->getPos(), // position
					1.0f // mass
				);

				ml::Rigidbody * rb = ent->add<ml::Rigidbody>(ML_Physics.createNewRigidbody(
					RB_BORG, transform, collider, particle
				));

				const ml::Material * material = ML_Res.materials.load_forward(
					"mat_borg",
					ML_Res.shaders.get("basic"),
					ml::List<ml::uni_base *>({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		ML_CAMERA->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		ML_CAMERA->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
						new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	ML_Res.textures.get("borg")),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ML_Res.models.get("default_cube"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, 1 } },
					})
				);
			}

			// Cube
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("cube"))
			{
				ml::Transform * transform = ent->add<ml::Transform>(
					ml::vec3 { 0.0f, 0.0f, -5.0f }, // position
					ml::vec3 { 0.5f }, // scale
					ml::quat { } // rotation
				);

				ml::BoxCollider * collider = ent->add<ml::BoxCollider>(
					transform->getScl() // size
				);

				ml::Particle * particle = ent->add<ml::Particle>(
					transform->getPos(), // position
					1.0f // mass
				);

				ml::Rigidbody * rb = ent->add<ml::Rigidbody>(ML_Physics.createNewRigidbody(
					RB_CUBE, transform, collider, particle
				));

				const ml::Material * material = ML_Res.materials.load_forward(
					"mat_cube",
					ML_Res.shaders.get("normal"),
					ml::List<ml::uni_base *>({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		ML_CAMERA->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		ML_CAMERA->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
						new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	ML_Res.textures.get("stone_dm")),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ML_Res.models.get("cube"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, 1 } },
					})
				);
			}

			// Navball
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("navball"))
			{
				ml::Transform * transform = ent->add<ml::Transform>(
					ml::vec3 { -5.0f, 0.0f, 0.0f }, // position
					ml::vec3 { 0.5f }, // scale
					ml::quat { } // rotation
				);

				ml::SphereCollider * collider = ent->add<ml::SphereCollider>(
					transform->getScl()[1] // radius
				);

				ml::Particle * particle = ent->add<ml::Particle>(
					transform->getPos(), // position
					1.0f // mass
				);

				ml::Rigidbody * rb = ent->add<ml::Rigidbody>(ML_Physics.createNewRigidbody(
					RB_NAVBALL, transform, collider, particle
				));

				const ml::Material * material = ML_Res.materials.load_forward(
					"mat_navball",
					ML_Res.shaders.get("basic"),
					ml::List<ml::uni_base *>({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		ML_CAMERA->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		ML_CAMERA->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
						new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	ML_Res.textures.get("navball")),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ML_Res.models.get("sphere32x24"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, 1 } },
						}
				));
			}

			// Moon
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("moon"))
			{
				ml::Transform * transform = ent->add<ml::Transform>(
					ml::vec3 { 0.0f, 0.0f, 5.0f }, // position
					ml::vec3 { 0.5f }, // scale
					ml::quat { } // rotation
				);

				ml::SphereCollider * collider = ent->add<ml::SphereCollider>(
					transform->getScl()[1] // radius
				);

				ml::Particle * particle = ent->add<ml::Particle>(
					transform->getPos(), // position
					1.0f // mass
				);

				ml::Rigidbody * rb = ent->add<ml::Rigidbody>(ML_Physics.createNewRigidbody(
					RB_MOON, transform, collider, particle
				));

				const ml::Material * material = ML_Res.materials.load_forward(
					"mat_moon",
					ML_Res.shaders.get("lighting"),
					ml::List<ml::uni_base *> ({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		ML_CAMERA->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		ML_CAMERA->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_vec3_cr	("Frag.cameraPos",	ML_CAMERA->get<ml::Transform>()->getPos()),
						new ml::uni_vec3_cr	("Frag.lightPos",	ML_LIGHT->get<ml::Transform>()->getPos()),
						new ml::uni_col4_cr	("Frag.diffuse",	ML_LIGHT->get<ml::Light>()->color),
						new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
						new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	ML_Res.textures.get("moon_dm")),
						new ml::uni_tex_cp	("Frag.specTex",	ML_Res.textures.get("moon_nm")),
						new ml::uni_flt		("Frag.ambient",	0.01f),
						new ml::uni_flt		("Frag.specular",	0.1f),
						new ml::uni_int		("Frag.shininess",	8),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ML_Res.models.get("sphere32x24"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, 1 } },
						}
				));
			}

			// Earth
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("earth"))
			{
				ml::Transform * transform = ent->add<ml::Transform>(
					ml::vec3 { 0.0f }, // position
					ml::vec3 { 1.0f }, // scale
					ml::quat { } // rotation
				);

				ml::SphereCollider * collider = ent->add<ml::SphereCollider>(
					transform->getScl()[0] // radius
				);
				collider->center_world = transform->getPos();

				ml::Particle * particle = ent->add<ml::Particle>(
					transform->getPos(), // position
					0.25f // mass
				);
				ml::Rigidbody * rb = ent->add<ml::Rigidbody>(ML_Physics.createNewRigidbody(
					RB_EARTH, transform, collider, particle
				));

				const ml::Material * material = ML_Res.materials.load_forward(
					"mat_earth",
					ML_Res.shaders.get("lighting"),
					ml::List<ml::uni_base *> ({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		ML_CAMERA->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		ML_CAMERA->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_vec3_cr	("Frag.cameraPos",	ML_CAMERA->get<ml::Transform>()->getPos()),
						new ml::uni_vec3_cr	("Frag.lightPos",	ML_LIGHT->get<ml::Transform>()->getPos()),
						new ml::uni_col4_cr	("Frag.diffuse",	ML_LIGHT->get<ml::Light>()->color),
						new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
						new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	ML_Res.textures.get("earth_dm")),
						new ml::uni_tex_cp	("Frag.specTex",	ML_Res.textures.get("earth_sm")),
						new ml::uni_flt		("Frag.ambient",	0.01f),
						new ml::uni_flt		("Frag.specular",	0.1f),
						new ml::uni_int		("Frag.shininess",	8),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ML_Res.models.get("sphere32x24"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, 1 } },
					}
				));
			}

			// Ground
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("ground"))
			{
				ml::Transform * transform = ent->add<ml::Transform>(
					ml::vec3 { 0.0f, -2.5f, 0.0f }, // position
					ml::vec3 { 12.5, 0.25f, 12.5 }, // scale
					ml::quat { } // rotation
				);

				ml::BoxCollider * collider = ent->add<ml::BoxCollider>(
					transform->getScl() // size
				);
				collider->center_world = transform->getPos();

				ml::Particle * particle = ent->add<ml::Particle>(
					transform->getPos(), // position
					1.0f // mass
				);

				ml::Rigidbody * rb = ent->add<ml::Rigidbody>(ML_Physics.createNewRigidbody(
					RB_GROUND, transform, collider, particle
				));

				const ml::Material * material = ML_Res.materials.load_forward(
					"mat_ground",
					ML_Res.shaders.get("normal"),
					ml::List<ml::uni_base *>({
						new ml::uni_mat4_cr	(ML_VERT_PROJ,		ML_CAMERA->get<ml::Camera>()->getPerspMatrix()),
						new ml::uni_mat4_cr	(ML_VERT_VIEW,		ML_CAMERA->get<ml::Transform>()->getMat()),
						new ml::uni_mat4_cr	(ML_VERT_MODEL,		transform->getMat()),
						new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
						new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	ML_Res.textures.get("stone_dm")),
						}));

				ml::Renderer * renderer = ent->add<ml::Renderer>(
					ML_Res.models.get("cube"),
					material,
					ml::RenderStates({
						{ ml::GL::AlphaTest,{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::DepthTest,{ ml::RenderVar::Bool, 1 } },
					})
				);
			}
		}

		// Launch Physics
		/* * * * * * * * * * * * * * * * * * * * */
		if (!ML_Physics.launch(demo_physics::init))
		{
			ml::Debug::fatal("Failed launching Physics");
		}
	}

	void Sandbox::onUpdate(const ml::UpdateEvent * ev)
	{
		// Sync Physics
		/* * * * * * * * * * * * * * * * * * * * */
		while (!ML_Physics.getCopyState(demo_physics::sync));

		// Update Std Out
		/* * * * * * * * * * * * * * * * * * * * */
		if (m_rdbuf)
		{
			ML_Terminal.printss(m_rdstr);
		}

		// Update Title
		/* * * * * * * * * * * * * * * * * * * * */
		this->setTitle(ml::String("{0} | {1} | {2} | {3} ms/frame ({4} fps)").format(
			SETTINGS.title,
			ML_CONFIGURATION,
			ML_PLATFORM_TARGET,
			ev->elapsed.delta(),
			ML_Engine.frameRate()
		));

		// Update Network
		/* * * * * * * * * * * * * * * * * * * * */
		if (SETTINGS.isServer)
		{
			ML_NetServer.poll();
		}
		else if (SETTINGS.isClient)
		{
			ML_NetClient.poll();
		}

		// Update Effects
		/* * * * * * * * * * * * * * * * * * * * */
		for (auto & pair : ML_Res.effects)
		{
			pair.second->resize(this->getFrameSize());
		}

		// Update Camera
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Camera * camera = ML_CAMERA->get<ml::Camera>())
		{
			camera->updateRes(this->getFrameSize());

			// Camera Transform
			if (ml::Transform * transform = ML_CAMERA->get<ml::Transform>())
			{
				// Target Entity
				if (const ml::Entity * ent = ML_Res.entities.get("earth"))
				{
					// Target Transform
					if (const ml::Transform * target = ent->get<ml::Transform>())
					{
						if (globals.camAuto)
						{
							camera->forward(target->getPos() - camera->position);

							transform->lookAt(camera->position, camera->forward());

							camera->position
								+=	camera->right()
								*	ev->elapsed.delta()
								*	globals.camSpd;
						}
					}
				}
			}
		}

		// Update Text
		/* * * * * * * * * * * * * * * * * * * * */
		{
			m_text["project_url"]
				.setFont(ML_Res.fonts.get("minecraft"))
				.setFontSize(56)
				.setPosition({ 48, (float)this->getFrameHeight() - 48 })
				.setString(ML_PROJECT_URL);

			const ml::Font *font	 = ML_Res.fonts.get("consolas");
			const uint32_t	fontSize = 24;
			const float		hOff	 = 0.0f;
			const float		vOff	 = 4.0f;
			const ml::vec2	offset	 = { hOff, (vOff + (float)fontSize) };
			const ml::vec2	origin	 = { (float)fontSize, (float)(fontSize * 2) };
			ml::vec2		linePos	 = 0.0f;
			size_t			lineNum	 = 0;
			auto			newLine  = [&]() { return (linePos = (origin + (offset * (float)(lineNum++)))); };

			m_text["gl_version"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("GL Version: {0}").format(
					ML_GL.getString(ml::GL::Version)));

			m_text["gl_vendor"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("GL Vendor: {0}").format(
					ML_GL.getString(ml::GL::Vendor)));

			newLine();

			m_text["framerate"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("{0} ms/frame ({1} fps)").format(
					ev->elapsed.delta(),
					ML_Engine.frameRate()));

			m_text["time_total"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("time: {0}").format(
					ML_Time.elapsed()));

			newLine();

			m_text["time_sin"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("sin: {0}").format(
					ML_Time.sin()));

			m_text["time_cos"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("cos: {0}").format(
					ML_Time.cos()));

			newLine();

			m_text["cursor_pos"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("cx/cy: {0}").format(
					this->getCursorPos()));

			m_text["window_pos"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("wx/wy: {0}").format(
					this->getPosition()));

			m_text["window_size"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(newLine())
				.setString(ml::String("ww/wh: {0}").format(
					this->getSize()));

			// Force Update All Text
			for (auto & pair : m_text)
			{
				pair.second.update();
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Sandbox::onDraw(const ml::DrawEvent * ev)
	{
		// Draw Scene
		/* * * * * * * * * * * * * * * * * * * * */
		if (const ml::Effect * scene = ML_Res.effects.get("frame_main"))
		{
			// Bind Scene
			scene->bind();

			// Clear Screen
			this->clear(ML_CAMERA->get<ml::Camera>()->color);

			// Draw Renderers
			for (const auto & pair : ML_Res.entities)
			{
				this->draw(pair.second->get<ml::Renderer>());
			}

			// Draw 2D
			/* * * * * * * * * * * * * * * * * * * * */
			{
				// Ortho Matrix
				const ml::mat4 & ortho = ML_CAMERA->get<ml::Camera>()->getOrthoMatrix();

				// Render States
				static ml::RenderStates states
				({
					{ ml::GL::AlphaTest,	{ ml::RenderVar::Bool, 1 } },
					{ ml::GL::Blend,		{ ml::RenderVar::Bool, 1 } },
					{ ml::GL::CullFace,		{ ml::RenderVar::Bool, 0 } },
					{ ml::GL::DepthTest,	{ ml::RenderVar::Bool, 0 } },
					{ ml::GL::Texture2D,	{ ml::RenderVar::Bool, 1 } },
				});
				states.apply();

				// Draw Sprites
				if (const ml::Shader * shader = ML_Res.shaders.get("sprites"))
				{
					static ml::Material * const material = ML_Res.materials.load_forward(
						"mat_sprites",
						shader,
						ml::List<ml::uni_base *>({
							new ml::uni_mat4_cr	(ML_VERT_PROJ,		ortho),
							new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
							new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	NULL),
							}));

					static ml::RenderBatch batch(&m_canvas.vao(), &m_canvas.vbo(), material);

					for (const auto & pair : ML_Res.sprites)
					{
						this->draw(pair.second, batch);
					}
				}

				// Draw Text
				if (const ml::Shader * shader = ML_Res.shaders.get("text"))
				{
					static ml::Material * const material = ML_Res.materials.load_forward(
						"mat_text",
						shader,
						ml::List<ml::uni_base *>({
							new ml::uni_mat4_cr	(ML_VERT_PROJ,		ortho),
							new ml::uni_col4	(ML_FRAG_MAIN_COL,	ml::Color::White),
							new ml::uni_tex_cp	(ML_FRAG_MAIN_TEX,	NULL),
							}));

					static ml::RenderBatch batch(&m_canvas.vao(), &m_canvas.vbo(), material);

					for (const auto & pair : m_text)
					{
						this->draw(pair.second, batch);
					}
				}

				// Draw Geometry
				if (const ml::Shader * shader = ML_Res.shaders.get("geometry"))
				{
					static ml::Material * const material = ML_Res.materials.load_forward(
						"mat_geometry", 
						shader,
						ml::List<ml::uni_base *>({
							new ml::uni_col4(ML_FRAG_MAIN_COL,	{ 0.385f, 0.0f, 1.0f, 1.0f }),
							new ml::uni_int("Geom.mode",		-1),
							new ml::uni_flt("Geom.delta",		1.0f),
							new ml::uni_flt("Geom.size",		0.995f),
							new ml::uni_int("Geom.samples",		16),
							}));

					if (material && material->bind())
					{
						ML_GL.drawArrays(ml::GL::Points, 0, 4);
						material->unbind();
					}
				}
			}

			// Unbind Scene
			scene->unbind();
		}

		// Draw Post
		/* * * * * * * * * * * * * * * * * * * * */
		if (const ml::Effect * post = ML_Res.effects.get("frame_post"))
		{
			post->bind();
			if (const ml::Effect * scene = ML_Res.effects.get("frame_main"))
			{
				scene->shader()->setUniform("Effect.mode", globals.effectMode);

				this->draw(*scene);
			}
			post->unbind();
		}
	}

	void Sandbox::onGui(const ml::GuiEvent * ev)
	{
		// Main Menu Bar
		if (ML_Editor.show_mainMenuBar)		{ ML_MainMenuBar.drawGui(); }
		
		// Dockspace
		if (ML_Editor.show_dockspace)		{ ML_Dockspace.drawGui(&ML_Editor.show_dockspace); }
		
		// ImGui Builtin
		if (ML_Editor.show_imgui_demo)		{ ml::ImGui_Builtin::showDemo(&ML_Editor.show_imgui_demo); }
		if (ML_Editor.show_imgui_metrics)	{ ml::ImGui_Builtin::showMetrics(&ML_Editor.show_imgui_metrics); }
		if (ML_Editor.show_imgui_style)		{ ml::ImGui_Builtin::showStyle(&ML_Editor.show_imgui_style); }
		if (ML_Editor.show_imgui_about)		{ ml::ImGui_Builtin::showAbout(&ML_Editor.show_imgui_about); }

		// Editors
		if (ML_Editor.show_network)			{ ML_NetworkHUD.drawGui(&ML_Editor.show_network); }
		if (ML_Editor.show_profiler)		{ ML_Profiler.drawGui(&ML_Editor.show_profiler); }
		if (ML_Editor.show_browser)			{ ML_Browser.drawGui(&ML_Editor.show_browser); }
		if (ML_Editor.show_resourceView)	{ ML_ResourceView.drawGui(&ML_Editor.show_resourceView); }
		if (ML_Editor.show_terminal)		{ ML_Terminal.drawGui(&ML_Editor.show_terminal); }
		if (ML_Editor.show_textEditor)		{ ML_TextEditor.drawGui(&ML_Editor.show_textEditor); }
		if (ML_Editor.show_builder)			{ ML_Builder.drawGui(&ML_Editor.show_builder); }
		
		// Scene View
		if (ML_Editor.show_sceneView)
		{
			ML_SceneView.drawFun(&ML_Editor.show_sceneView, [&]()
			{
				if (ml::Effect * post = ML_Res.effects.get("frame_post"))
				{
					ML_SceneView.updateTexture(&post->texture());
				}
			});
		}

		// Inspector
		if (ML_Editor.show_inspector)
		{
			ML_Inspector.drawFun(&ML_Editor.show_inspector, [&]()
			{
				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::Text("Framebuffer");
				static ml::CString fbo_modes[] = {
					"Normal",
					"Grayscale",
					"Blur",
					"Juicy",
					"Inverted",
				};
				ImGui::Combo("Shader##Framebuffer", &globals.effectMode, fbo_modes, IM_ARRAYSIZE(fbo_modes));
				ImGui::Separator();

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::Text("Camera");
				ImGui::Checkbox("Auto##Camera", &globals.camAuto);
				ImGui::DragFloat("Speed##Camera", &globals.camSpd, 0.1f, -5.f, 5.f);
				ImGui::Separator();

				/* * * * * * * * * * * * * * * * * * * * */
			});
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Sandbox::onUnload(const ml::UnloadEvent * ev)
	{
		ml::Debug::log("Unloading...");

		// Cleanup Std Out
		if (m_rdbuf) 
		{ 
			ml::cout.rdbuf(m_rdbuf);
			m_rdbuf = NULL; 
		}

		// Cleanup Resources
		ML_Res.dispose();

		// Cleanup Physics Thread
		ML_Physics.dispose();
	}

	void Sandbox::onExit(const ml::ExitEvent * ev)
	{
		ml::Debug::log("Exiting...");

		// Shutdown ImGui
		ImGui_ML_Shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}