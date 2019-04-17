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
#include <MemeGraphics/RenderStates.hpp>
#include <MemeGraphics/Renderer.hpp>
#include <MemeEditor/Editor.hpp>
#include <MemeEditor/EditorCommands.hpp>
#include <MemeEditor/EditorEvents.hpp>
#include <MemeEditor/GUI.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEngine/ECS.hpp>
#include <MemeEngine/EngineCommands.hpp>
#include <MemeEngine/Resources.hpp>
#include <MemeEngine/Preferences.hpp>
#include <MemeNet/NetClient.hpp>
#include <MemeNet/NetServer.hpp>
#include <MemePhysics/Physics.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeWindow/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_FBO_MAIN "fbo_main"
#define ML_FBO_POST "fbo_post"

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Sandbox::Sandbox() 
		: EditorApplication() 
	{
	}

	Sandbox::~Sandbox() 
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	void Sandbox::onEvent(const ml::IEvent * value)
	{
		// Handle base events
		ml::EditorApplication::onEvent(value);

		switch (*value)
		{
			// File -> Close ...
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::EditorEvent::EV_File_Close:
			if (const auto * ev = value->as<ml::File_Close_Event>())
			{
				this->close();
			}
			break;

			// File -> Open ...
			/* * * * * * * * * * * * * * * * * * * * */
		case ml::EditorEvent::EV_File_Open:
			if (const auto * ev = value->as<ml::File_Open_Event>())
			{
				if (ML_Editor.show_browser)
				{
					ML_OS.execute("open", ML_Browser.get_selected_path());
				}
			}
			break;
			
			// Key Event
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

				// Reload Shaders (Num1)
				if (ev->getKeyDown(ml::KeyCode::Num1))
				{
					ml::Debug::log("Reloaded {0} Shaders.", ML_Res.shaders.reload());
				}

				// Toggle Smooth Textures (Num2)
				if (ev->getKeyDown(ml::KeyCode::Num2))
				{
					for (auto pair : ML_Res.textures)
					{
						pair.second->setSmooth(!pair.second->smooth());
					}
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
		ml::Debug::log("Entering...");

		// Setup Miscellaneous
		/* * * * * * * * * * * * * * * * * * * * */
		{
			// Start Master Timer
			ML_Time.start();

			// Seed Random
			ml::Random::seed();

			// GL Error Pause
			ML_GL.errorPause(SETTINGS.glErrorPause);
		}

		// Setup Interpreter
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
			if (scr.loadFromFile(ML_FS.pathTo(SETTINGS.bootScript)))
			{
				if (!(scr.buildAndRun(ml::Args(ev->argc, ev->argv))))
				{
					ml::Debug::logError("Failed Running \'{0}\'", SETTINGS.bootScript);
				}
			}
		}

		// Create Window
		/* * * * * * * * * * * * * * * * * * * * */
		if (this->create(SETTINGS.title, SETTINGS.video(), SETTINGS.style, SETTINGS.context()) &&
			this->setup(SETTINGS.glExperimental))
		{
			this->setInputMode(ml::Cursor::Normal);
			this->setPosition((ml::VideoMode::desktop().size - this->getSize()) / 2);
			this->setViewport(ml::vec2i::Zero, this->getFramebufferSize());
			this->maximize();
		}
		else
		{
			ml::Debug::fatal("Failed Creating Window");
		}

		// Load ImGui
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Debug::log("Dear ImGui..."))
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGui::StyleColorsDark();
			ImGui::GetStyle().FrameBorderSize = 1;
			if (!ImGui_ML_Init("#version 410", this, true, SETTINGS.imguiINI.c_str()))
			{
				ml::Debug::fatal("Failed Loading ImGui");
			}
		}

		// Load Audio
		/* * * * * * * * * * * * * * * * * * * * */
		if (!(ML_AL.init()))
		{
			ml::Debug::fatal("Failed Loading OpenAL");
		}

		// Load Network
		/* * * * * * * * * * * * * * * * * * * * */
		if (SETTINGS.isServer)
		{
			// Server Setup
			ml::Debug::log("Starting Server...");
			if (ML_NetServer.setup())
			{
				if (ML_NetServer.start({ ML_LOCALHOST, ML_PORT }, ML_MAX_CLIENTS))
				{
					ml::Debug::log("Server Started: {0}", ML_NetServer.getMyAddress());
				}
			}
		}
		else if (SETTINGS.isClient)
		{
			// Client Setup
			ml::Debug::log("Starting Client...");
			if (ML_NetClient.setup())
			{
				if (ML_NetClient.connect({ ML_LOCALHOST, ML_PORT }))
				{
					ml::Debug::log("Client Connected: {0}", ML_NetClient.getMyAddress());
				}
			}
		}
	}

	void Sandbox::onLoad(const ml::LoadEvent * ev)
	{
		ml::Debug::log("Loading...");

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
		if (!ML_Res.loadFromFile(ML_FS.pathTo(SETTINGS.manifest)))
		{
			ml::Debug::logError("Failed Loading Manifest");
		}

		// Setup Canvas
		/* * * * * * * * * * * * * * * * * * * * */
		if (!m_canvas.create())
		{
			ml::Debug::logError("Failed Creating Canvas");
		}
	}

	void Sandbox::onStart(const ml::StartEvent * ev)
	{
		ml::Debug::log("Starting...");

		// Set Window Icon
		/* * * * * * * * * * * * * * * * * * * * */
		if (const ml::Image * icon = ML_Res.images.get("icon"))
		{
			ml::Image temp = ml::Image(*icon).flipVertically();

			this->setIcons({ temp });
		}

		// Setup Plugins
		/* * * * * * * * * * * * * * * * * * * * */
		{
			if (ml::Plugin * p = ML_Res.plugins.get("TestPlugin"))
			{
				p->lib().callFunction<void *>(
					ML_STRINGIFY(ML_Plugin_Test),
					"Hello from Test Plugin!"
					);
			}
		}

		// Setup Camera
		/* * * * * * * * * * * * * * * * * * * * */
		{
			data.camera.lookAt(
				data.camPos,
				data.camPos + ml::vec3f::Back,
				ml::vec3f::Up);
		}

		// Setup Sprites
		/* * * * * * * * * * * * * * * * * * * * */
		{
			if (ml::Sprite * s = ML_Res.sprites.get("neutrino"))
			{
				(*s).setPosition((ml::vec2f(0.95f, 0.075f) * this->getSize()))
					.setScale(0.5f)
					.setRotation(0.0f)
					.setOrigin(0.5f)
					.setColor(ml::Color::White);
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
					ml::RenderFlags
					{
						{ ml::GL::CullFace, true },
						{ ml::GL::DepthTest, true },
					},
					ml::UniformSet
					{
						{ "Vert.proj",		ml::Uniform::Mat4,	&data.persp.matrix() },
						{ "Vert.view",		ml::Uniform::Mat4,	&data.camera.matrix() },
						{ "Vert.model",		ml::Uniform::Mat4,	&transform->matrix() },
						{ "Frag.mainCol",	ml::Uniform::Vec4,	&data.lightCol },
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
					ml::RenderFlags
					{
						{ ml::GL::CullFace, true },
						{ ml::GL::DepthTest, true },
					},
					ml::UniformSet
					{
						{ "Vert.proj",		ml::Uniform::Mat4,	&data.persp.matrix() },
						{ "Vert.view",		ml::Uniform::Mat4,	&data.camera.matrix() },
						{ "Vert.model",		ml::Uniform::Mat4,	&transform->matrix() },
						{ "Frag.mainCol",	ml::Uniform::Vec4,	&ml::Color::White },
						{ "Frag.mainTex",	ml::Uniform::Tex2D,	ML_Res.textures.get("borg") },
					}
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
					ml::RenderFlags
					{
						{ ml::GL::CullFace, false },
						{ ml::GL::DepthTest, true },
					},
					ml::UniformSet
					{
						{ "Vert.proj",		ml::Uniform::Mat4,	&data.persp.matrix() },
						{ "Vert.view",		ml::Uniform::Mat4,	&data.camera.matrix() },
						{ "Vert.model",		ml::Uniform::Mat4,	&transform->matrix() },
						{ "Frag.mainCol",	ml::Uniform::Vec4,	&ml::Color::White },
						{ "Frag.mainTex",	ml::Uniform::Tex2D,	ML_Res.textures.get("sanic") },
					}
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
					ml::RenderFlags
					{
						{ ml::GL::CullFace, true },
						{ ml::GL::DepthTest, true },
					},
					ml::UniformSet
					{
						{ "Vert.proj",		ml::Uniform::Mat4,	&data.persp.matrix() },
						{ "Vert.view",		ml::Uniform::Mat4,	&data.camera.matrix() },
						{ "Vert.model",		ml::Uniform::Mat4,	&transform->matrix() },
						{ "Frag.mainCol",	ml::Uniform::Vec4,	&ml::Color::White },
						{ "Frag.mainTex",	ml::Uniform::Tex2D,	ML_Res.textures.get("stone_dm") },
					}
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
					ml::RenderFlags
					{
						{ ml::GL::CullFace, true },
						{ ml::GL::DepthTest, true },
					},
					ml::UniformSet
					{
						{ "Vert.proj",		ml::Uniform::Mat4,	&data.persp.matrix() },
						{ "Vert.view",		ml::Uniform::Mat4,	&data.camera.matrix() },
						{ "Vert.model",		ml::Uniform::Mat4,	&transform->matrix() },
						{ "Frag.mainCol",	ml::Uniform::Vec4,	&ml::Color::White },
						{ "Frag.mainTex",	ml::Uniform::Tex2D,	ML_Res.textures.get("stone_dm") },
					}
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
					ml::RenderFlags
					{
						{ ml::GL::CullFace, true },
						{ ml::GL::DepthTest, true },
					},
					ml::UniformSet
					{
						{ "Vert.proj",		ml::Uniform::Mat4,	&data.persp.matrix() },
						{ "Vert.view",		ml::Uniform::Mat4,	&data.camera.matrix() },
						{ "Vert.model",		ml::Uniform::Mat4,	&transform->matrix() },
						{ "Frag.tex_dm",	ml::Uniform::Tex2D,	ML_Res.textures.get("moon_dm") },
						{ "Frag.tex_sm",	ml::Uniform::Tex2D,	ML_Res.textures.get("moon_nm") },
						{ "Frag.camPos",	ml::Uniform::Vec3,	&data.camPos },
						{ "Frag.lightPos",	ml::Uniform::Vec3,	&data.lightPos },
						{ "Frag.lightCol",	ml::Uniform::Vec4,	&data.lightCol },
						{ "Frag.ambient",	ml::Uniform::Float, &data.ambient },
						{ "Frag.specular",	ml::Uniform::Float, &data.specular },
						{ "Frag.shininess",	ml::Uniform::Int,	&data.shininess },
					}
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
					ml::RenderFlags
					{
						{ ml::GL::CullFace, true },
						{ ml::GL::DepthTest, true },
					},
					ml::UniformSet 
					{
						{ "Vert.proj",		ml::Uniform::Mat4,	&data.persp.matrix() },
						{ "Vert.view",		ml::Uniform::Mat4,	&data.camera.matrix() },
						{ "Vert.model",		ml::Uniform::Mat4,	&transform->matrix() },
						{ "Frag.tex_dm",	ml::Uniform::Tex2D,	ML_Res.textures.get("earth_dm") },
						{ "Frag.tex_sm",	ml::Uniform::Tex2D,	ML_Res.textures.get("earth_sm") },
						{ "Frag.camPos",	ml::Uniform::Vec3,	&data.camPos },
						{ "Frag.lightPos",	ml::Uniform::Vec3,	&data.lightPos },
						{ "Frag.lightCol",	ml::Uniform::Vec4,	&data.lightCol },
						{ "Frag.ambient",	ml::Uniform::Float, &data.ambient },
						{ "Frag.specular",	ml::Uniform::Float, &data.specular },
						{ "Frag.shininess",	ml::Uniform::Int,	&data.shininess },
					}
				});
			}
		}

		// Setup Physics
		/* * * * * * * * * * * * * * * * * * * * */
		ML_Physics.thread().launch([&]()
		{
			/* * * * * * * * * * * * * * * * * * * * */

			ML_Physics.world().state().resize(ML_Res.entities.size());

			/* * * * * * * * * * * * * * * * * * * * */

			while (this->isOpen())
			{
				ml::PhysicsState state;
				if (ML_Physics.beginUpdate(state))
				{
					for (size_t i = 0, imax = state.size(); i < imax; i++)
					{
						ml::vec3f	pos; // Position
						ml::quat	rot; // Rotation
						ml::mat4f	mat; // Transform
						ml::mat4f	inv; // Inverse Transform
						if (state.getData(i, pos, rot, mat, inv))
						{
							if (i == 0)
							{
								pos[1] = +ML_Time.cos(); // just updating one object for testing purposes
							}

							state.setData(i, pos, rot, mat, inv);
						}
					}

					ML_Physics.endUpdate(state);
				}
			}

			/* * * * * * * * * * * * * * * * * * * * */
		});
	}

	void Sandbox::onUpdate(const ml::UpdateEvent * ev)
	{
		// Update Window Title
		/* * * * * * * * * * * * * * * * * * * * */
		this->setTitle(ml::String("{0} | {1} | {2} | {3} ms/frame ({4} fps)").format(
			SETTINGS.title,
			ml::Debug::configuration(),
			ml::Debug::platformTarget(),
			ev->elapsed.delta(),
			ML_Time.calculateFPS(ev->elapsed.delta())
		));

		// Update Network
		/* * * * * * * * * * * * * * * * * * * * */
		{
			if (SETTINGS.isServer)
			{
				ML_NetServer.poll();
			}
			else if (SETTINGS.isClient)
			{
				ML_NetClient.poll();
			}
		}

		// Update Entities
		/* * * * * * * * * * * * * * * * * * * * */
		{
			// Light
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("light"))
			{
				ent->get<ml::Transform>()
					->setPosition(data.lightPos)
					.rotate(-ev->elapsed.delta(), ml::vec3f::Forward);
			}

			// Borg
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("borg"))
			{
				// FIXME: this is a hack, needs to update PhysicsState a bit
				static bool once;
				if (!once && (once = true))
				{
					ML_Physics.world().state().setPosition(0, ent->get<ml::Transform>()->getPosition());
				}

				ml::vec3f pos; 
				ML_Physics.world().state().getPosition(0, pos);

				ent->get<ml::Transform>()
					->setPosition(pos)
					.rotate(+ev->elapsed.delta(), ml::vec3f::One);
			}

			// Cube
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("cube"))
			{
				ml::vec3f pos = ent->get<ml::Transform>()->getPosition();
				pos[1] = -ML_Time.sin();
				ent->get<ml::Transform>()
					->setPosition(pos)
					.rotate(-ev->elapsed.delta(), ml::vec3f::One);
			}

			// Sanic
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("sanic"))
			{
				ml::vec3f pos = ent->get<ml::Transform>()->getPosition();
				pos[1] = -ML_Time.cos();
				ent->get<ml::Transform>()
					->setPosition(pos)
					.rotate(-ev->elapsed.delta(), ml::vec3f::Forward);
			}

			// Moon
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("moon"))
			{
				ml::vec3f pos = ent->get<ml::Transform>()->getPosition();
				pos[1] = +ML_Time.sin();
				ent->get<ml::Transform>()
					->setPosition(pos)
					.rotate(-ev->elapsed.delta(), ml::vec3f::Up);
			}

			// Earth
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Entity * ent = ML_Res.entities.get("earth"))
			{
				ent->get<ml::Transform>()
					->rotate(ev->elapsed.delta(), ml::vec3f::Up);
			}
		}

		// Update Camera
		/* * * * * * * * * * * * * * * * * * * * */
		if (const ml::Entity * target = ML_Res.entities.get("earth"))
		{
			// Look
			ml::vec3f pos = target->get<ml::Transform>()->getPosition();
			ml::vec3f dir = (pos - data.camPos).normalized();
			ml::vec3f look = data.camPos + (pos - data.camPos).normalized();
			data.camera.lookAt(data.camPos, look, ml::vec3f::Up);

			// Orbit
			float speed = (data.camAnimate ? data.camSpd * ev->elapsed.delta() : 0.0f);
			ml::vec3f fwd = (look - data.camPos);
			ml::vec3f right = (fwd.cross(ml::vec3f::Up) * ml::vec3f(1, 0, 1)).normalized();
			data.camPos += right * speed;
		}

		// Update Text
		/* * * * * * * * * * * * * * * * * * * * */
		{
			m_text["project_url"]
				.setFont(ML_Res.fonts.get("minecraft"))
				.setFontSize(56)
				.setPosition({ 48 })
				.setString(SETTINGS.projectURL);

			const ml::Font *font = ML_Res.fonts.get("consolas");
			uint32_t		fontSize = 18;
			float			hOff = 0.0f;
			float			vOff = 4.0f;
			ml::vec2f		offset = { hOff, -(vOff + (float)fontSize) };
			ml::vec2f		origin = { (float)fontSize, (float)this->height() - 48 };
			ml::vec2f		linePos = 0;
			size_t			lineNum = 0;
			auto			nextLine = [&]() { return linePos = (origin + (offset * (float)(lineNum++))); };

			m_text["gl_version"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(nextLine())
				.setString(ml::String("GL Version: {0}").format(
					ML_GL.getString(ml::GL::Version)));


			m_text["gl_vendor"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(nextLine())
				.setString(ml::String("GL Vendor: {0}").format(
					ML_GL.getString(ml::GL::Vendor)));

			nextLine();

			m_text["fps_str"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(nextLine())
				.setString(ml::String("{0} ms/frame ({1} fps)").format(
					ev->elapsed.delta(),
					ML_Time.calculateFPS(ev->elapsed.delta())));

			m_text["time"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(nextLine())
				.setString(ml::String("time: {0}").format(
					ML_Time.elapsed()));

			nextLine();

			m_text["sin"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(nextLine())
				.setString(ml::String("sin: {0}").format(
					ML_Time.sin()));

			m_text["cos"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(nextLine())
				.setString(ml::String("cos: {0}").format(
					ML_Time.cos()));

			nextLine();

			m_text["cursor_pos"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(nextLine())
				.setString(ml::String("cx/cy: {0}").format(
					this->getCursorPos()));

			m_text["window_pos"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(nextLine())
				.setString(ml::String("wx/wy: {0}").format(
					this->getPosition()));

			m_text["window_size"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(nextLine())
				.setString(ml::String("ww/wh: {0}").format(
					this->getSize()));

			// Force Update All Text
			for (auto pair : m_text)
			{
				pair.second.update();
			}
		}
	}

	void Sandbox::onDraw(const ml::DrawEvent * ev)
	{
		// Update Resolution
		/* * * * * * * * * * * * * * * * * * * * */
		ml::vec2i resolution;
		if ((resolution = this->getFramebufferSize()) != ml::vec2i::Zero)
		{
			// Resize Effects
			bool changed = false;
			for (auto pair : ML_Res.effects)
			{
				if (pair.second->resize(resolution))
				{
					changed = true;
				}
			}
			if (changed)
			{
				// Orthographic
				data.ortho.orthographic(
					{ ml::vec2f::Zero, (ml::vec2f)resolution },
					{ SETTINGS.orthoNear, SETTINGS.orthoFar }
				);

				// Perspective
				data.persp.perspective(
					SETTINGS.fieldOfView, ((float)resolution[0] / (float)resolution[1]),
					SETTINGS.perspNear, SETTINGS.perspFar
				);
			}
		}

		// Draw Scene
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Effect * scene = ML_Res.effects.get(ML_FBO_MAIN))
		{
			// Bind Scene
			/* * * * * * * * * * * * * * * * * * * * */
			scene->bind();

			// Clear Screen
			/* * * * * * * * * * * * * * * * * * * * */
			this->clear(data.clearColor);

			// Draw Entities
			/* * * * * * * * * * * * * * * * * * * * */
			for (auto & pair : ML_Res.entities)
			{
				if (ml::Renderer * renderer = pair.second->get<ml::Renderer>())
				{
					this->draw(*renderer);
				}
			}

			// Draw 2D
			/* * * * * * * * * * * * * * * * * * * * */
			{
				ML_GL.disable(ml::GL::CullFace);
				ML_GL.disable(ml::GL::DepthTest);

				// Geometry
				/* * * * * * * * * * * * * * * * * * * * */
				if (const ml::Shader * shader = ML_Res.shaders.get("geometry"))
				{
					static ml::UniformSet uniforms = 
					{
						{ "Frag.mainCol",	ml::Uniform::Vec4,	&data.lineColor },
						{ "Curve.mode",		ml::Uniform::Int,	&data.lineMode },
						{ "Curve.delta",	ml::Uniform::Float, &data.lineDelta },
						{ "Curve.size",		ml::Uniform::Float, &data.lineSize },
						{ "Curve.samples",	ml::Uniform::Int,	&data.lineSamples },
					};
					shader->applyUniforms(uniforms);
					shader->bind();
					ML_GL.drawArrays(ml::GL::Points, 0, 4);
				}

				// Sprites
				/* * * * * * * * * * * * * * * * * * * * */
				if (const ml::Shader * shader = ML_Res.shaders.get("sprites"))
				{
					static ml::RenderBatch batch(
						&m_canvas.vao(),
						&m_canvas.vbo(),
						shader,
						ml::UniformSet
						{
							{ "Vert.proj",		ml::Uniform::Mat4,	&data.ortho.matrix() },
							{ "Frag.mainCol",	ml::Uniform::Vec4 },
							{ "Frag.mainTex",	ml::Uniform::Tex2D },
						}
					);

					for (auto & pair : ML_Res.sprites)
					{
						this->draw((*pair.second), batch);
					}
				}

				// Text
				/* * * * * * * * * * * * * * * * * * * * */
				if (const ml::Shader * shader = ML_Res.shaders.get("text"))
				{
					static ml::RenderBatch batch(
						&m_canvas.vao(),
						&m_canvas.vbo(), 
						shader, 
						ml::UniformSet 
						{
							{ "Vert.proj",		ml::Uniform::Mat4,	&data.ortho.matrix() },
							{ "Frag.mainCol",	ml::Uniform::Vec4 },
							{ "Frag.mainTex",	ml::Uniform::Tex2D },
						}
					);

					for (auto & pair : m_text)
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
		if (ml::Effect * post = ML_Res.effects.get(ML_FBO_POST))
		{
			if (ml::Effect * scene = ML_Res.effects.get(ML_FBO_MAIN))
			{
				if (const ml::Shader * shader = scene->shader())
				{
					post->bind();
					shader->applyUniforms({
						{ "Effect.mode", ml::Uniform::Int, &data.effectMode },
					});
					this->draw(*scene);
					post->unbind();
				}
			}
		}
	}

	void Sandbox::onGui(const ml::GuiEvent * ev)
	{
		// Main Menu Bar
		if (ML_Editor.show_mainMenuBar)		{ drawMainMenuBar(); }
		// Dockspace
		if (ML_Editor.show_dockspace)		{ drawDockspace(&ML_Editor.show_dockspace); }
		// ImGui Builtin
		if (ML_Editor.show_imgui_demo)		{ ml::ImGui_Builtin::showDemo(&ML_Editor.show_imgui_demo); }
		if (ML_Editor.show_imgui_metrics)	{ ml::ImGui_Builtin::showMetrics(&ML_Editor.show_imgui_metrics); }
		if (ML_Editor.show_imgui_style)		{ ml::ImGui_Builtin::showStyle(&ML_Editor.show_imgui_style); }
		if (ML_Editor.show_imgui_about)		{ ml::ImGui_Builtin::showAbout(&ML_Editor.show_imgui_about); }
		// Editors
		if (ML_Editor.show_network)			{ ML_NetworkHUD.drawGui(&ML_Editor.show_network); }
		if (ML_Editor.show_profiler)		{ ML_Profiler.drawGui(&ML_Editor.show_profiler); }
		if (ML_Editor.show_project)			{ ML_Project.drawGui(&ML_Editor.show_project); }
		if (ML_Editor.show_browser)			{ ML_Browser.drawGui(&ML_Editor.show_browser); }
		if (ML_Editor.show_terminal)		{ ML_Terminal.drawGui(&ML_Editor.show_terminal); }
		if (ML_Editor.show_textEditor)		{ ML_TextEditor.drawGui(&ML_Editor.show_textEditor); }
		if (ML_Editor.show_builder)			{ ML_Builder.drawGui(&ML_Editor.show_builder); }
		if (ML_Editor.show_sceneView)		{ drawSceneView(&ML_Editor.show_sceneView); }
		if (ML_Editor.show_inspector)		{ drawInspector(&ML_Editor.show_inspector); }
	}

	void Sandbox::onUnload(const ml::UnloadEvent * ev)
	{
		ml::Debug::log("Unloading...");

		// Unload Resources
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

	bool Sandbox::drawMainMenuBar()
	{
		// Draw Main Menu Bar
		return ML_MainMenuBar.drawFun([&]()
		{
			// File Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					ML_EventSystem.fireEvent(ml::File_New_Event());
				}
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					ML_EventSystem.fireEvent(ml::File_Open_Event());
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					ML_EventSystem.fireEvent(ml::File_Save_Event(false));
				}
				if (ImGui::MenuItem("Save All", "Ctrl+Shift+S", false))
				{
					ML_EventSystem.fireEvent(ml::File_Save_Event(true));
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Quit", "Alt+F4"))
				{
					ML_EventSystem.fireEvent(ml::File_Close_Event());
				}
				ImGui::EndMenu();
			}
			// Edit Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z"))
				{
					ML_EventSystem.fireEvent(ml::Edit_Undo_Event());
				}
				if (ImGui::MenuItem("Redo", "Ctrl+Y"))
				{
					ML_EventSystem.fireEvent(ml::Edit_Redo_Event());
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "Ctrl+X"))
				{
					ML_EventSystem.fireEvent(ml::Edit_Cut_Event());
				}
				if (ImGui::MenuItem("Copy", "Ctrl+C"))
				{
					ML_EventSystem.fireEvent(ml::Edit_Copy_Event());
				}
				if (ImGui::MenuItem("Paste", "Ctrl+V"))
				{
					ML_EventSystem.fireEvent(ml::Edit_Paste_Event());
				}
				ImGui::EndMenu();
			}
			// Window Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem(ML_Terminal.title(), "Ctrl+Alt+T", &ML_Editor.show_terminal);
				ImGui::MenuItem(ML_Browser.title(), "Ctrl+Alt+E", &ML_Editor.show_browser);
				ImGui::MenuItem(ML_Builder.title(), "Ctrl+Alt+B", &ML_Editor.show_builder);
				ImGui::MenuItem(ML_SceneView.title(), "Ctrl+Alt+S", &ML_Editor.show_sceneView);
				ImGui::MenuItem(ML_Inspector.title(), "Ctrl+Alt+I", &ML_Editor.show_inspector);
				ImGui::MenuItem(ML_TextEditor.title(), NULL, &ML_Editor.show_textEditor);
				ImGui::MenuItem(ML_Project.title(), NULL, &ML_Editor.show_project);
				ImGui::MenuItem(ML_NetworkHUD.title(), NULL, &ML_Editor.show_network);
				ImGui::EndMenu();
			}
			// Help Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Project Page"))
				{
					ML_OS.execute("open", SETTINGS.projectURL);
				}
				ImGui::Separator();
				ImGui::MenuItem("ImGui Demo", "Ctrl+H", &ML_Editor.show_imgui_demo);
				ImGui::MenuItem("ImGui Metrics", NULL, &ML_Editor.show_imgui_metrics);
				ImGui::MenuItem("ImGui Style Editor", NULL, &ML_Editor.show_imgui_style);
				ImGui::MenuItem("About Dear ImGui", NULL, &ML_Editor.show_imgui_about);
				ImGui::EndMenu();
			}
		});
	}

	bool Sandbox::drawDockspace(bool * p_open)
	{
		// Draw Dockspace
		return ML_Dockspace.drawFun(p_open, [&]()
		{
			// Dockspace Builder
			if (uint32_t root = ML_Dockspace.beginBuilder(ImGuiDockNodeFlags_None))
			{
				uint32_t left = ML_Dockspace.splitNode(root, ImGuiDir_Left, 0.29f, &root);
				uint32_t center = ML_Dockspace.splitNode(root, ImGuiDir_Right, 0.5f, &root);
				uint32_t right = ML_Dockspace.splitNode(center, ImGuiDir_Right, 0.21f, &center);

				const uint32_t left_U = ML_Dockspace.splitNode(left, ImGuiDir_Up, 0.65f, &left);
				const uint32_t left_D = ML_Dockspace.splitNode(left, ImGuiDir_Down, 0.35f, &left);
				const uint32_t center_U = ML_Dockspace.splitNode(center, ImGuiDir_Up, 0.65f, &center);
				const uint32_t center_D = ML_Dockspace.splitNode(center, ImGuiDir_Down, 0.35f, &center);
				const uint32_t right_U = ML_Dockspace.splitNode(right, ImGuiDir_Up, 0.65f, &right);
				const uint32_t right_D = ML_Dockspace.splitNode(right, ImGuiDir_Down, 0.35f, &right);

				ML_Dockspace.dockWindow(ML_Profiler.title(), left_U);
				ML_Dockspace.dockWindow(ML_NetworkHUD.title(), left_U);
				ML_Dockspace.dockWindow(ML_Browser.title(), left_U);
				ML_Dockspace.dockWindow(ML_Project.title(), left_U);
				ML_Dockspace.dockWindow(ML_Terminal.title(), left_D);
				ML_Dockspace.dockWindow(ML_SceneView.title(), center_U);
				ML_Dockspace.dockWindow(ML_Builder.title(), center_D);
				ML_Dockspace.dockWindow(ML_TextEditor.title(), center_D);
				ML_Dockspace.dockWindow(ML_Inspector.title(), right_U);

				ML_Dockspace.endBuilder(root);
			};
		});
	}

	bool Sandbox::drawSceneView(bool * p_open)
	{
		// Draw Scene View
		return ML_SceneView.drawFun(p_open, [&]()
		{
			if (ml::Effect * post = ML_Res.effects.get(ML_FBO_POST))
			{
				ML_SceneView.updateTexture(&post->texture());
			}
		});
	}

	bool Sandbox::drawInspector(bool * p_open)
	{
		// Draw Inspector
		return ML_Inspector.drawFun(p_open, [&]()
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
			ImGui::Checkbox("Move##Camera", &data.camAnimate);
			ml::GUI::EditVec3f("Position##Camera", data.camPos);
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}