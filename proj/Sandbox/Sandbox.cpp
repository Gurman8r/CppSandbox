/* * * * * * * * * * * * * * * * * * * * */

#include "Sandbox.hpp"
#include "Settings.hpp"

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
#include <MemeEditor/Editor.hpp>
#include <MemeEditor/EditorCommands.hpp>
#include <MemeEditor/EditorEvents.hpp>
#include <MemeEditor/GUI.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEditor/ImGui_Style.hpp>
#include <MemeEditor/StyleLoader.hpp>
#include <MemeEngine/Engine.hpp>
#include <MemeEngine/Entity.hpp>
#include <MemeEngine/EngineCommands.hpp>
#include <MemeEngine/Resources.hpp>
#include <MemeEngine/Preferences.hpp>
#include <MemeNet/NetClient.hpp>
#include <MemeNet/NetServer.hpp>
#include <MemePhysics/Physics.hpp>
#include <MemePhysics/Rigidbody.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeWindow/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * */

enum Rigidbody_ID : int32_t
{
	DEMO_BORG,
	DEMO_CUBE,
	DEMO_SANIC,
	DEMO_MOON,
	DEMO_EARTH,
	DEMO_GROUND,

	MAX_RIGIDBODY_ID
};

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

			// Mouse Button Input
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::WindowEvent::EV_MouseButton:
			if (const auto * ev = value->as<ml::MouseButtonEvent>())
			{
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

			// Setup Std Out Redirect
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
		// Setup Canvas
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
		// Set Window Icon
		/* * * * * * * * * * * * * * * * * * * * */
		if (const ml::Image * icon = ML_Res.images.get("icon"))
		{
			const ml::Image temp = ml::Image(*icon).flipVertically();

			this->setIcons({ temp });
		}

		// Setup Plugins
		/* * * * * * * * * * * * * * * * * * * * */
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

		// Setup Camera
		/* * * * * * * * * * * * * * * * * * * * */
		{
			m_camera.position()		= { 0.0f, 1.0f, 10.0f };
			m_camera.fieldOfView()	= SETTINGS.fieldOfView;
			m_camera.perspNear()	= SETTINGS.perspNear;
			m_camera.perspFar()		= SETTINGS.perspFar;
			m_camera.orthoNear()	= SETTINGS.orthoNear;
			m_camera.orthoFar()		= SETTINGS.orthoFar;

			m_camera.transform() = ml::Transform::LookAt(
				m_camera.position(),
				m_camera.position() + ml::vec3f::Back,
				ml::vec3f::Up);
		}

		// Setup Sprites
		/* * * * * * * * * * * * * * * * * * * * */
		{
			if (ml::Sprite * s = ML_Res.sprites.get("neutrino"))
			{
				(*s).setPosition(ml::vec2f(0.95f, 0.075f) * this->getSize())
					.setScale	(0.5f)
					.setRotation(0.0f)
					.setOrigin	(0.5f)
					.setColor	(ml::Color::White);
			}
		}

		// Setup Entities
		/* * * * * * * * * * * * * * * * * * * * */
		{
			// Light
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.load("light"))
			{
				ml::Transform * transform = ent->add<ml::Transform>({
					data.lightPos, // position
					{ 1.0f }, // scale
					{ } // rotation
				});

				ml::Renderer * renderer = ent->add<ml::Renderer>({
					ML_Res.models.get("light"),
					ML_Res.shaders.get("solid"),
					ml::RenderStates
					{
						{ ml::GL::AlphaTest,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,		{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,		{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::DepthTest,	{ ml::RenderVar::Bool, 1 } },
					},
					ml::UniformSet
					{
						{ ML_VERT_PROJ,		ml::Uniform::Mat4,	&m_camera.persp().matrix() },
						{ ML_VERT_VIEW,		ml::Uniform::Mat4,	&m_camera.transform().matrix() },
						{ ML_VERT_MODEL,	ml::Uniform::Mat4,	&transform->matrix() },
						{ ML_FRAG_MAIN_COL,	ml::Uniform::Vec4,	&data.lightCol },
					}
				});
			}

			// Borg
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.load("borg"))
			{
				ml::Transform * transform = ent->add<ml::Transform>({
					{ 5.0f, 0.0f, 0.0f }, // position
					{ 1.0f }, // scale
					{ } // rotation
				});

				ml::Renderer * renderer = ent->add<ml::Renderer>({
					ML_Res.models.get("borg"),
					ML_Res.shaders.get("basic"),
					ml::RenderStates
					{
						{ ml::GL::AlphaTest,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,		{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,		{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::DepthTest,	{ ml::RenderVar::Bool, 1 } },
					},
					ml::UniformSet
					{
						{ ML_VERT_PROJ,		ml::Uniform::Mat4,	&m_camera.persp().matrix() },
						{ ML_VERT_VIEW,		ml::Uniform::Mat4,	&m_camera.transform().matrix() },
						{ ML_VERT_MODEL,	ml::Uniform::Mat4,	&transform->matrix() },
						{ ML_FRAG_MAIN_COL,	ml::Uniform::Vec4,	&ml::Color::White },
						{ ML_FRAG_MAIN_TEX,	ml::Uniform::Tex2D,	ML_Res.textures.get("borg") },
					}
				});

				ml::Rigidbody * rigidbody = ent->add<ml::Rigidbody>({
					DEMO_BORG, transform
				});
			}

			// Cube
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.load("cube"))
			{
				ml::Transform * transform = ent->add<ml::Transform>({
					{ 0.0f, 0.0f, -5.0f }, // position
					{ 0.5f }, // scale
					{ } // rotation
				});

				ml::Renderer * renderer = ent->add<ml::Renderer>({
					ML_Res.models.get("cube"),
					ML_Res.shaders.get("normal"),
					ml::RenderStates
					{
						{ ml::GL::AlphaTest,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,		{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,		{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::DepthTest,	{ ml::RenderVar::Bool, 1 } },
					},
					ml::UniformSet
					{
						{ ML_VERT_PROJ,		ml::Uniform::Mat4,	&m_camera.persp().matrix() },
						{ ML_VERT_VIEW,		ml::Uniform::Mat4,	&m_camera.transform().matrix() },
						{ ML_VERT_MODEL,	ml::Uniform::Mat4,	&transform->matrix() },
						{ ML_FRAG_MAIN_COL,	ml::Uniform::Vec4,	&ml::Color::White },
						{ ML_FRAG_MAIN_TEX,	ml::Uniform::Tex2D,	ML_Res.textures.get("stone_dm") },
					}
				});

				ml::Rigidbody * rigidbody = ent->add<ml::Rigidbody>({
					DEMO_CUBE, transform
				});
			}

			// Sanic
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.load("sanic"))
			{
				ml::Transform * transform = ent->add<ml::Transform>({
					{ -5.0f, 0.0f, 0.0f }, // position
					{ 1.0f }, // scale
					{ } // rotation
				});

				ml::Renderer * renderer = ent->add<ml::Renderer>({
					ML_Res.models.get("sanic"),
					ML_Res.shaders.get("basic"),
					ml::RenderStates
					{
						{ ml::GL::AlphaTest,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,		{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,		{ ml::RenderVar::Bool, 0 } },
						{ ml::GL::DepthTest,	{ ml::RenderVar::Bool, 1 } },
					},
					ml::UniformSet
					{
						{ ML_VERT_PROJ,		ml::Uniform::Mat4,	&m_camera.persp().matrix() },
						{ ML_VERT_VIEW,		ml::Uniform::Mat4,	&m_camera.transform().matrix() },
						{ ML_VERT_MODEL,	ml::Uniform::Mat4,	&transform->matrix() },
						{ ML_FRAG_MAIN_COL,	ml::Uniform::Vec4,	&ml::Color::White },
						{ ML_FRAG_MAIN_TEX,	ml::Uniform::Tex2D,	ML_Res.textures.get("sanic") },
					}
				});

				ml::Rigidbody * rigidbody = ent->add<ml::Rigidbody>({
					DEMO_SANIC, transform
				});
			}

			// Moon
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.load("moon"))
			{
				ml::Transform * transform = ent->add<ml::Transform>({
					{ 0.0f, 0.0f, 5.0f }, // position
					{ 0.5f }, // scale
					{ } // rotation
				});

				ml::Renderer * renderer = ent->add<ml::Renderer>({
					ML_Res.models.get("moon"),
					ML_Res.shaders.get("lighting"),
					ml::RenderStates
					{
						{ ml::GL::AlphaTest,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,		{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,		{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::DepthTest,	{ ml::RenderVar::Bool, 1 } },
					},
					ml::UniformSet
					{
						{ ML_VERT_PROJ,		ml::Uniform::Mat4,	&m_camera.persp().matrix() },
						{ ML_VERT_VIEW,		ml::Uniform::Mat4,	&m_camera.transform().matrix() },
						{ ML_VERT_MODEL,	ml::Uniform::Mat4,	&transform->matrix() },
						{ ML_FRAG_MAIN_TEX,	ml::Uniform::Tex2D,	ML_Res.textures.get("moon_dm") },
						{ "Frag.specTex",	ml::Uniform::Tex2D,	ML_Res.textures.get("moon_nm") },
						{ "Frag.camPos",	ml::Uniform::Vec3,	&m_camera.position() },
						{ "Frag.lightPos",	ml::Uniform::Vec3,	&data.lightPos },
						{ "Frag.lightCol",	ml::Uniform::Vec4,	&data.lightCol },
						{ "Frag.ambient",	ml::Uniform::Float, &data.ambient },
						{ "Frag.specular",	ml::Uniform::Float, &data.specular },
						{ "Frag.shininess",	ml::Uniform::Int,	&data.shininess },
					}
				});

				ml::Rigidbody * rigidbody = ent->add<ml::Rigidbody>({
					DEMO_MOON, transform
				});
			}

			// Earth
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.load("earth"))
			{
				ml::Transform * transform = ent->add<ml::Transform>({
					{ 0.0f }, // position
					{ 1.0f }, // scale
					{ } // rotation
				});

				ml::Renderer * renderer = ent->add<ml::Renderer>({
					ML_Res.models.get("earth"),
					ML_Res.shaders.get("lighting"),
					ml::RenderStates
					{
						{ ml::GL::AlphaTest,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,		{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,		{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::DepthTest,	{ ml::RenderVar::Bool, 1 } },
					},
					ml::UniformSet
					{
						{ ML_VERT_PROJ,		ml::Uniform::Mat4,	&m_camera.persp().matrix() },
						{ ML_VERT_VIEW,		ml::Uniform::Mat4,	&m_camera.transform().matrix() },
						{ ML_VERT_MODEL,	ml::Uniform::Mat4,	&transform->matrix() },
						{ ML_FRAG_MAIN_TEX,	ml::Uniform::Tex2D,	ML_Res.textures.get("earth_dm") },
						{ "Frag.specTex",	ml::Uniform::Tex2D,	ML_Res.textures.get("earth_sm") },
						{ "Frag.camPos",	ml::Uniform::Vec3,	&m_camera.position() },
						{ "Frag.lightPos",	ml::Uniform::Vec3,	&data.lightPos },
						{ "Frag.lightCol",	ml::Uniform::Vec4,	&data.lightCol },
						{ "Frag.ambient",	ml::Uniform::Float, &data.ambient },
						{ "Frag.specular",	ml::Uniform::Float, &data.specular },
						{ "Frag.shininess",	ml::Uniform::Int,	&data.shininess },
					}
				});

				ml::Rigidbody * rigidbody = ent->add<ml::Rigidbody>({
					DEMO_EARTH, transform
				});
			}

			// Ground
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.load("ground"))
			{
				ml::Transform * transform = ent->add<ml::Transform>({
					{ 0.0f, -2.5f, 0.0f }, // position
					{ 12.5, 0.25f, 12.5 }, // scale
					{ } // rotation
				});

				ml::Renderer * renderer = ent->add<ml::Renderer>({
					ML_Res.models.get("ground"),
					ML_Res.shaders.get("normal"),
					ml::RenderStates
					{
						{ ml::GL::AlphaTest,	{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::Blend,		{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::CullFace,		{ ml::RenderVar::Bool, 1 } },
						{ ml::GL::DepthTest,	{ ml::RenderVar::Bool, 1 } },
					},
					ml::UniformSet
					{
						{ ML_VERT_PROJ,		ml::Uniform::Mat4,	&m_camera.persp().matrix() },
						{ ML_VERT_VIEW,		ml::Uniform::Mat4,	&m_camera.transform().matrix() },
						{ ML_VERT_MODEL,	ml::Uniform::Mat4,	&transform->matrix() },
						{ ML_FRAG_MAIN_COL,	ml::Uniform::Vec4,	&ml::Color::White },
						{ ML_FRAG_MAIN_TEX,	ml::Uniform::Tex2D,	ML_Res.textures.get("stone_dm") },
					}
				});

				ml::Rigidbody * rigidbody = ent->add<ml::Rigidbody>({
					DEMO_GROUND, transform
				});
			}
		}

		// Setup Physics
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Physics.thread().launch([&]()
		{
			/* * * * * * * * * * * * * * * * * * * * */

			ML_Physics.world().state().resize(MAX_RIGIDBODY_ID);

			for (const auto & pair : ML_Res.entities)
			{
				if (const ml::Rigidbody * rb = pair.second->get<ml::Rigidbody>())
				{
					ML_Physics.world().state().setData(
						rb->index(), 
						rb->transform()->getPosition(),
						ml::quat(),
						rb->transform()->matrix(),
						rb->transform()->getInverse()
					);
				}
			}

			/* * * * * * * * * * * * * * * * * * * * */

			auto updatePhysics = [](const float dt)
			{
				ml::PhysicsState state;
				if (ML_Physics.beginUpdate(state))
				{
					for (int32_t i = 0; i < state.size(); i++)
					{
						ml::vec3f	pos;
						ml::quat	rot;
						ml::mat4f	mat;
						ml::mat4f	inv;
						if (state.getData(i, pos, rot, mat, inv))
						{
							switch (i)
							{
							case DEMO_BORG:
								pos = { pos[0], +ML_Time.cos(), pos[2] };
								rot = { ml::vec3f::One, +dt };
								break;

							case DEMO_CUBE:
								pos = { pos[0], -ML_Time.sin(), pos[2] };
								rot = { ml::vec3f::One, -dt };
								break;

							case DEMO_SANIC:
								pos = { pos[0], -ML_Time.cos(), pos[2] };
								rot = { ml::vec3f::Forward, -dt };
								break;

							case DEMO_MOON:
								pos = { pos[0], +ML_Time.sin(), pos[2] };
								rot = { ml::vec3f::Up, -dt };
								break;

							case DEMO_EARTH:
								rot = { ml::vec3f::Up, +dt };
								break;

							case DEMO_GROUND:
								break;
							}

							if (!state.setData(i, pos, rot, mat, inv))
							{
								ml::Debug::logError("Failed setting physics state: {0}", i);
							}
						}
					}
					ML_Physics.endUpdate(state);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * */

			while (this->isOpen())
			{
				updatePhysics(ML_Engine.frameTime().delta());
			}

			/* * * * * * * * * * * * * * * * * * * * */
		});
	}

	void Sandbox::onUpdate(const ml::UpdateEvent * ev)
	{
		// Update Std Out Redirect
		/* * * * * * * * * * * * * * * * * * * * */
		if (m_rdbuf)
		{
			ML_Terminal.printss(m_rdstr);
		}

		// Update Window Title
		/* * * * * * * * * * * * * * * * * * * * */
		{
			this->setTitle(ml::String("{0} | {1} | {2} | {3} ms/frame ({4} fps)").format(
				SETTINGS.title,
				ML_CONFIGURATION,
				ML_PLATFORM,
				ev->elapsed.delta(),
				ML_Engine.frameRate()
			));
		}

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
		{
			m_camera.update(this->getFrameSize());

			if (const ml::Entity * target = ML_Res.entities.get("earth"))
			{
				if (const ml::Transform * transform = target->get<ml::Transform>())
				{
					m_camera.orbit(
						(transform->getPosition()),
						(data.camAnim ? data.camSpd * ev->elapsed.delta() : 0.0f)
					);
				}
			}
		}

		// Update Entities
		/* * * * * * * * * * * * * * * * * * * * */
		{
			// Light
			if (ml::Entity * ent = ML_Res.entities.get("light"))
			{
				if (ml::Transform * transform = ent->get<ml::Transform>())
				{
					transform->setPosition(data.lightPos);
					transform->rotate(-ev->elapsed.delta(), ml::vec3f::Forward);
				}
			}

			// Physics
			for (auto & pair : ML_Res.entities)
			{
				if (ml::Rigidbody * rigidbody = pair.second->get<ml::Rigidbody>())
				{
					if (ml::Transform * transform = rigidbody->transform())
					{
						ml::vec3f pos;
						ml::quat  rot;
						ml::mat4f mat;
						ml::mat4f inv;
						if (ML_Physics.world().state().getData(
							rigidbody->index(), pos, rot, mat, inv
						))
						{
							transform->setPosition(pos);

							// FIXME: this is not how you use quaternions
							transform->rotate(rot.real(), rot.complex());
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
				.setPosition({ 48 })
				.setString(ML_PROJECT_URL);

			const ml::Font *font	 = ML_Res.fonts.get("consolas");
			const uint32_t	fontSize = 24;
			const float		hOff	 = 0.0f;
			const float		vOff	 = 4.0f;
			const ml::vec2f	offset	 = { hOff, -(vOff + (float)fontSize) };
			const ml::vec2f	origin	 = { (float)fontSize, (float)this->getHeight() - (fontSize * 2) };
			ml::vec2f		linePos	 = 0.0f;
			size_t			lineNum	 = 0;
			auto			newLine = [&]() { return (linePos = (origin + (offset * (float)(lineNum++)))); };

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

	void Sandbox::onDraw(const ml::DrawEvent * ev)
	{
		// Draw Scene
		/* * * * * * * * * * * * * * * * * * * * */
		if (const ml::Effect * scene = ML_Res.effects.get("frame_main"))
		{
			// Bind Scene
			/* * * * * * * * * * * * * * * * * * * * */
			scene->bind();

			// Clear Screen
			/* * * * * * * * * * * * * * * * * * * * */
			this->clear(data.clearColor);

			// Draw Entities
			/* * * * * * * * * * * * * * * * * * * * */
			for (const auto & pair : ML_Res.entities)
			{
				if (const ml::Renderer * renderer = pair.second->get<ml::Renderer>())
				{
					this->draw(*renderer);
				}
			}

			// Draw 2D
			/* * * * * * * * * * * * * * * * * * * * */
			{
				static ml::RenderStates states =
				{
					{ ml::GL::AlphaTest,	{ ml::RenderVar::Bool, 1 } },
					{ ml::GL::Blend,		{ ml::RenderVar::Bool, 1 } },
					{ ml::GL::CullFace,		{ ml::RenderVar::Bool, 0 } },
					{ ml::GL::DepthTest,	{ ml::RenderVar::Bool, 0 } },
				};
				states.apply();

				// Draw Geometry
				/* * * * * * * * * * * * * * * * * * * * */
				if (const ml::Shader * shader = ML_Res.shaders.get("geometry"))
				{
					static ml::UniformSet uniforms = 
					{
						{ ML_FRAG_MAIN_COL,	ml::Uniform::Vec4,	&data.lineColor },
						{ "Geom.mode",		ml::Uniform::Int,	&data.lineMode },
						{ "Geom.delta",		ml::Uniform::Float, &data.lineDelta },
						{ "Geom.size",		ml::Uniform::Float, &data.lineSize },
						{ "Geom.samples",	ml::Uniform::Int,	&data.lineSamples },
					};
					shader->applyUniforms(uniforms);
					shader->bind();
					ML_GL.drawArrays(ml::GL::Points, 0, 4);
				}

				// Draw Sprites
				/* * * * * * * * * * * * * * * * * * * * */
				if (const ml::Shader * shader = ML_Res.shaders.get("sprites"))
				{
					static ml::RenderBatch batch(&m_canvas.vao(), &m_canvas.vbo(), shader,
						{
							{ ML_VERT_PROJ,		ml::Uniform::Mat4,	&m_camera.ortho().matrix() },
							{ ML_FRAG_MAIN_COL,	ml::Uniform::Vec4 },
							{ ML_FRAG_MAIN_TEX,	ml::Uniform::Tex2D },
						}
					);
					for (const auto & pair : ML_Res.sprites)
					{
						this->draw((*pair.second), batch);
					}
				}

				// Draw Text
				/* * * * * * * * * * * * * * * * * * * * */
				if (const ml::Shader * shader = ML_Res.shaders.get("text"))
				{
					static ml::RenderBatch batch(&m_canvas.vao(), &m_canvas.vbo(), shader, 
						{
							{ ML_VERT_PROJ,		ml::Uniform::Mat4,	&m_camera.ortho().matrix() },
							{ ML_FRAG_MAIN_COL,	ml::Uniform::Vec4 },
							{ ML_FRAG_MAIN_TEX,	ml::Uniform::Tex2D },
						}
					);
					for (const auto & pair : m_text)
					{
						this->draw(pair.second, batch);
					}
				}
			}

			// Unbind Scene
			/* * * * * * * * * * * * * * * * * * * * */
			scene->unbind();
		}

		// Draw Post Processing
		/* * * * * * * * * * * * * * * * * * * * */
		if (const ml::Effect * post = ML_Res.effects.get("frame_post"))
		{
			if (const ml::Effect * scene = ML_Res.effects.get("frame_main"))
			{
				if (const ml::Shader * shader = scene->shader())
				{
					static ml::UniformSet uniforms = 
					{
						{ "Effect.mode", ml::Uniform::Int, &data.effectMode },
					};
					post->bind();
					shader->applyUniforms(uniforms);
					this->draw(*scene);
					post->unbind();
				}
			}
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

				if (ImGui::BeginMenuBar())
				{
					ImGui::Text("%s", ML_Inspector.title());
					ImGui::EndMenuBar();
				}

				/* * * * * * * * * * * * * * * * * * * * */

				if (ImGui::Button("Reload Shaders"))
				{
					ml::Debug::log("Reloaded {0} Shaders.", ML_Res.shaders.reload());
				}
				ImGui::Separator();

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::Text("Scene");
				ImGui::ColorEdit4("Color##Scene", &data.clearColor[0]);
				ImGui::Separator();

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::Text("Framebuffer");
				static ml::CString fbo_modes[] = {
					"Normal",
					"Grayscale",
					"Blur",
					"Juicy",
					"Inverted",
				};
				ImGui::Combo("Shader##Framebuffer", &data.effectMode, fbo_modes, IM_ARRAYSIZE(fbo_modes));
				ImGui::Separator();

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::Text("Camera");
				ImGui::Checkbox("Move##Camera", &data.camAnim);
				ml::GUI::EditVec3f("Position##Camera", m_camera.position());
				ImGui::DragFloat("Speed##Camera", &data.camSpd, 0.1f, -5.f, 5.f);
				ImGui::Separator();

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::Text("Light");
				ml::GUI::EditVec3f("Position##Light", data.lightPos);
				ImGui::ColorEdit4("Color##Light", &data.lightCol[0]);
				ImGui::DragFloat("Ambient##Light", &data.ambient, 0.01f, 0.f, 1.f);
				ImGui::DragFloat("Specular##Light", &data.specular, 0.01f, 0.1f, 10.f);
				ImGui::DragInt("Shininess##Light", &data.shininess, 1.f, 1, 256);
				ImGui::Separator();

				/* * * * * * * * * * * * * * * * * * * * */

				ImGui::Text("Geometry");
				ImGui::SliderInt("Mode##Geometry", &data.lineMode, -1, 3);
				ImGui::ColorEdit4("Color##Geometry", &data.lineColor[0]);
				ImGui::SliderFloat("Delta##Geometry", &data.lineDelta, 0.f, 1.f);
				ImGui::SliderFloat("Size##Geometry", &data.lineSize, 0.f, 1.f);
				ImGui::SliderInt("Samples##Geometry", &data.lineSamples, 1, 128);
				ImGui::Separator();

				/* * * * * * * * * * * * * * * * * * * * */
			});
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Sandbox::onUnload(const ml::UnloadEvent * ev)
	{
		ml::Debug::log("Unloading...");

		// Cleanup Std Out Redirect
		if (m_rdbuf) 
		{ 
			ml::cout.rdbuf(m_rdbuf);
			m_rdbuf = NULL; 
		}

		// Cleanup Resources
		ML_Res.dispose();

		// Cleanup Physics Thread
		ML_Physics.thread().dispose();
	}

	void Sandbox::onExit(const ml::ExitEvent * ev)
	{
		ml::Debug::log("Exiting...");

		// Shutdown ImGui
		ImGui_ML_Shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}