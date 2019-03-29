/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Demo.hpp"
#include "DemoCommands.hpp"
#include <MemeAudio/Audio.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/Dispatcher.hpp>
#include <MemeCore/Time.hpp>
#include <MemeCore/FileSystem.hpp> 
#include <MemeCore/Random.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeCore/OS.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeGraphics/Camera.hpp>
#include <MemeEditor/ResourceManager.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEditor/EditorEvents.hpp>
#include <MemeEditor/MainMenuBar.hpp>
#include <MemeEditor/GUI.hpp>
#include <MemeEditor/Terminal.hpp>
#include <MemeEditor/Builder.hpp>
#include <MemeEditor/Browser.hpp>
#include <MemeEditor/ImGui_Builtin.hpp>
#include <MemeEditor/Dockspace.hpp>
#include <MemeEditor/TextEditor.hpp>
#include <MemeEditor/Hierarchy.hpp>
#include <MemeEditor/SceneView.hpp>
#include <MemeEditor/Inspector.hpp>
#include <MemeEditor/ResourceHUD.hpp>
#include <MemeEditor/NetworkHUD.hpp>
#include <MemeEditor/Profiler.hpp>
#include <MemeEditor/ShaderTool.hpp>
#include <MemeNet/Client.hpp>
#include <MemeNet/Server.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeWindow/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_FBO_MAIN "fbo_main"
#define ML_FBO_POST "fbo_post"

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * */

	Demo::Demo()
	{
		ML_EventSystem.addListener(DemoEvent::EV_Enter, this);
		ML_EventSystem.addListener(DemoEvent::EV_Load, this);
		ML_EventSystem.addListener(DemoEvent::EV_Start, this);
		ML_EventSystem.addListener(DemoEvent::EV_FixedUpdate, this);
		ML_EventSystem.addListener(DemoEvent::EV_Update, this);
		ML_EventSystem.addListener(DemoEvent::EV_Draw, this);
		ML_EventSystem.addListener(DemoEvent::EV_Gui, this);
		ML_EventSystem.addListener(DemoEvent::EV_Unload, this);
		ML_EventSystem.addListener(DemoEvent::EV_Exit, this);

		ML_EventSystem.addListener(ml::CoreEvent::EV_RequestExit, this);

		ML_EventSystem.addListener(ml::EditorEvent::EV_Inspector, this);
	}

	Demo::~Demo() {}

	/* * * * * * * * * * * * * * * * * * * * */

	void Demo::onEvent(const ml::IEvent * value)
	{
		ml::RenderWindow::onEvent(value);

		switch (value->eventID())
		{
		case DemoEvent::EV_Enter:		return onEnter(*value->as<EnterEvent>());
		case DemoEvent::EV_Load:		return onLoad(*value->as<LoadEvent>());
		case DemoEvent::EV_Start:		return onStart(*value->as<StartEvent>());
		case DemoEvent::EV_FixedUpdate:	return onFixedUpdate(*value->as<FixedUpdateEvent>());
		case DemoEvent::EV_Update:		return onUpdate(*value->as<UpdateEvent>());
		case DemoEvent::EV_Draw:		return onDraw(*value->as<DrawEvent>());
		case DemoEvent::EV_Gui:			return onGui(*value->as<GuiEvent>());
		case DemoEvent::EV_Unload:		return onUnload(*value->as<UnloadEvent>());
		case DemoEvent::EV_Exit:		return onExit(*value->as<ExitEvent>());

		case ml::CoreEvent::EV_RequestExit:
			if (const auto * ev = value->as<ml::RequestExitEvent>())
				this->close();
			break;

		case ml::WindowEvent::EV_FramebufferSize:
			if (const auto * ev = value->as<ml::FramebufferSizeEvent>())
				this->setViewport(ml::vec2i::Zero, ev->size());
			break;

		case ml::WindowEvent::EV_Key:
			if (const auto * ev = value->as<ml::KeyEvent>())
			{
				/* * * * * * * * * * * * * * * * * * * * */

				// Mods
				const bool mod_ctrl = (ev->mods & ML_MOD_CTRL);
				const bool mod_alt = (ev->mods & ML_MOD_ALT);
				const bool mod_shift = (ev->mods & ML_MOD_SHIFT);

				/* * * * * * * * * * * * * * * * * * * * */

				// Reload Shaders (Num1)
				if (ev->getKeyDown(ml::KeyCode::Num1))
				{
					ml::Debug::log("Reloaded {0} Shaders.", ML_Res.shaders.reload());
				}

				// Toggle Smooth Textures (Num2)
				if (ev->getKeyDown(ml::KeyCode::Num2))
				{
					for (auto pair : ML_Res.textures.getAll())
					{
						pair.second->setSmooth(!pair.second->smooth());
					}
				}

				// Close (Escape)
				if (ev->getKeyDown(ml::KeyCode::Escape))
				{
					if (SETTINGS.escapeIsExit) { this->close(); }
				}

				/* * * * * * * * * * * * * * * * * * * * */

				// Terminal (Ctrl+Alt+T)
				if (ev->getKeyDown(ml::KeyCode::T) && (mod_ctrl && mod_alt))
					gui.show_terminal = true;

				// Browser (Ctrl+Alt+E)
				if (ev->getKeyDown(ml::KeyCode::E) && (mod_ctrl))
					gui.show_browser = true;

				// Builder (Ctrl+Alt+B)
				if (ev->getKeyDown(ml::KeyCode::B) && (mod_ctrl && mod_alt))
					gui.show_builder = true;

				// Scene (Ctrl+Alt+S)
				if (ev->getKeyDown(ml::KeyCode::S) && (mod_ctrl && mod_alt))
					gui.show_scene = true;

				// Inspector (Ctrl+Alt+I)
				if (ev->getKeyDown(ml::KeyCode::I) && (mod_ctrl && mod_alt))
					gui.show_inspector = true;

				// ImGui Demo (Ctrl+H)
				if (ev->getKeyDown(ml::KeyCode::H) && (mod_ctrl))
					gui.show_imgui_demo = true;

				/* * * * * * * * * * * * * * * * * * * * */
			}
			break;

		case ml::EditorEvent::EV_Inspector:
			if (const auto * ev = value->as<ml::InspectorEvent>())
			{
				if (ImGui::BeginMenuBar())
				{
					ImGui::Text("%s (WIP)", ML_Inspector.title());
					ImGui::EndMenuBar();
				}
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Demo::onEnter(const EnterEvent & ev)
	{
		ml::Debug::log("Entering...");

		// Start Master Timer
		ML_Time.start();

		// Seed Random
		ml::Random::seed();

		// GL Error Pause
		ml::OpenGL::errorPause(SETTINGS.glErrorPause);

		// Setup Interpreter
		/* * * * * * * * * * * * * * * * * * * * */
		{
			// Set Parser Flags
			ML_Parser.showToks(SETTINGS.flagShowToks);
			ML_Parser.showTree(SETTINGS.flagShowTree);
			ML_Parser.showItoP(SETTINGS.flagShowItoP);
			
			// Load Commands
			install_commands();
			
			// Run Boot Script
			ml::Script scr;
			if (scr.loadFromFile(ML_FileSystem.pathTo(SETTINGS.bootScript)))
			{
				if (scr.build(ev.args))
				{
					if (scr.run()) {}
				}
			}
		}

		// Create Window
		/* * * * * * * * * * * * * * * * * * * * */
		if (this->create(SETTINGS.title, SETTINGS.video(), SETTINGS.style, SETTINGS.context())
			&&
			this->setup(SETTINGS.glExperimental))
		{
			this->setInputMode(ml::Cursor::Normal);
			this->setPosition((ml::VideoMode::desktop().size - this->getSize()) / 2);
			this->setViewport(ml::vec2i::Zero, this->getFramebufferSize());
			this->maximize();

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
					return ml::Debug::setError(ml::Debug::logError("Failed Loading ImGui"));
				}
			}

			// Load OpenAL
			/* * * * * * * * * * * * * * * * * * * * */
			if (!ml::OpenAL::init())
			{
				return ml::Debug::setError(ml::Debug::logError("Failed Loading OpenAL"));
			}

			// Load Network
			/* * * * * * * * * * * * * * * * * * * * */
			if (SETTINGS.isServer)
			{
				// Server Setup
				ml::Debug::log("Starting Server...");
				if (ML_Server.setup())
				{
					if (ML_Server.start({ ML_LOCALHOST, ML_PORT }, ML_MAX_CLIENTS))
					{
						ml::Debug::log("Server Started: {0}", ML_Server.getMyAddress());
					}
				}
			}
			else if (SETTINGS.isClient)
			{
				// Client Setup
				ml::Debug::log("Starting Client...");
				if (ML_Client.setup())
				{
					if (ML_Client.connect({ ML_LOCALHOST, ML_PORT }))
					{
						ml::Debug::log("Client Connected: {0}", ML_Client.getMyAddress());
					}
				}
			}
		}
		else
		{
			return ml::Debug::setError(ml::Debug::logError(
				"Failed Loading Window\n"
				"Try checking your settings:\n"
				"\"{0}\"\n",
				SETTINGS.configINI));
		}
	}

	void Demo::onLoad(const LoadEvent & ev)
	{
		ml::Debug::log("Loading...");

		// Batch
		/* * * * * * * * * * * * * * * * * * * * */
		m_batchVAO.create(ml::GL::Triangles).bind();
		m_batchVBO.create(ml::GL::DynamicDraw).bind();
		m_batchVBO.bufferData(NULL, ml::RectQuad::Size);
		ml::BufferLayout::Default.bind();
		m_batchVBO.unbind();
		m_batchVAO.unbind();

		// Default Meshes
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

		// Default Models
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
		if (!ML_Res.loadFromFile(ML_FileSystem.pathTo(SETTINGS.manifest)))
		{
			ml::Debug::logError("Failed Loading Manifest");
		}
	}

	void Demo::onStart(const StartEvent & ev)
	{
		ml::Debug::log("Starting...");

		// Window Icon
		if (ml::Image * icon = ML_Res.images.get("icon"))
		{
			this->setIcons({ (*icon).flipVertically() });
		}

		// Camera
		/* * * * * * * * * * * * * * * * * * * * */
		{
			uni.camera.lookAt(uni.camPos, uni.camPos + ml::vec3f::Back, ml::vec3f::Up);
		}

		// Models
		/* * * * * * * * * * * * * * * * * * * * */
		{
			if (ml::Model * m = ML_Res.models.get("borg"))
				m->transform()
				.translate({ +5.0f, 0.0f, 0.0f })
				.scale(1.0f);

			if (ml::Model * m = ML_Res.models.get("sanic"))
				m->transform()
				.translate({ -5.0f, 0.0f, 0.0f })
				.scale(1.0f);

			if (ml::Model * m = ML_Res.models.get("earth"))
				m->transform()
				.translate({ 0.0f, 0.0f, 0.0f })
				.scale(1.0f);

			if (ml::Model * m = ML_Res.models.get("cube"))
				m->transform()
				.translate({ 0.0f, 0.0f, -5.0f })
				.scale(0.5f);

			if (ml::Model * m = ML_Res.models.get("moon"))
				m->transform()
				.translate({ 0.0f, 0.0f, 5.0f })
				.scale(0.5f);

			if (ml::Model * m = ML_Res.models.get("ground"))
				m->transform()
				.translate({ 0.0f, -2.5f, 0.0f })
				.scale({ 12.5, 0.25f, 12.5 });

			if (ml::Model * m = ML_Res.models.get("default_skybox"))
				m->transform()
				.translate({ 0.0f, 0.0f, 0.0f })
				.scale(10.f);
		}

		// Sprites
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

		// Plugins
		/* * * * * * * * * * * * * * * * * * * * */
		{
			if (ml::Plugin * p = ML_Res.plugins.get("TestPlugin"))
			{
				p->init("TestPlugin Init");
				p->enable("TestPlugin Enable");
				p->disable("TestPlugin Disable");
			}
		}
	}

	void Demo::onFixedUpdate(const FixedUpdateEvent & ev)
	{
		// Update Physics
		/* * * * * * * * * * * * * * * * * * * * */
		{
			for (ml::Particle & p : phys.particles)
			{
				p.applyForce(ml::Force::gravity(ml::vec3f::Up, p.mass));
			}
		}
	}

	void Demo::onUpdate(const UpdateEvent & ev)
	{
		// Poll Events
		/* * * * * * * * * * * * * * * * * * * * */
		this->pollEvents();

		// Update Title
		/* * * * * * * * * * * * * * * * * * * * */
		this->setTitle(ml::String::Format("{0} | {1} | {2} | {3} ms/frame ({4} fps)",
			SETTINGS.title,
			ml::Debug::configuration(),
			ml::Debug::platformTarget(),
			ev.elapsed.delta(),
			ML_Time.calculateFPS(ev.elapsed.delta()
			)
		));

		// Update Models
		/* * * * * * * * * * * * * * * * * * * * */
		{
			if (ml::Model * m = ML_Res.models.get("borg"))
			{
				ml::vec3f pos = m->transform().getPosition();
				pos[1] = +ML_Time.cos();
				m->transform()
					.setPosition(pos)
					.rotate(+ev.elapsed.delta(), ml::vec3f::One);
			}

			if (ml::Model * m = ML_Res.models.get("cube"))
			{
				ml::vec3f pos = m->transform().getPosition();
				pos[1] = -ML_Time.sin();
				m->transform()
					.setPosition(pos)
					.rotate(-ev.elapsed.delta(), ml::vec3f::One);
			}

			if (ml::Model * m = ML_Res.models.get("earth"))
			{
				m->transform()
					.rotate((uni.animate ? ev.elapsed.delta() : 0.f), ml::vec3f::Up);
			}

			if (ml::Model * m = ML_Res.models.get("moon"))
			{
				ml::vec3f pos = m->transform().getPosition();
				pos[1] = +ML_Time.sin();
				m->transform()
					.setPosition(pos)
					.rotate(-ev.elapsed.delta(), ml::vec3f::Up);
			}

			if (ml::Model * m = ML_Res.models.get("sanic"))
			{
				ml::vec3f pos = m->transform().getPosition();
				pos[1] = -ML_Time.cos();
				m->transform()
					.setPosition(pos)
					.rotate(-ev.elapsed.delta(), ml::vec3f::Forward);
			}

			if (ml::Model * m = ML_Res.models.get("light"))
			{
				m->transform()
					.setPosition(uni.lightPos)
					.rotate(-ev.elapsed.delta(), ml::vec3f::Forward);
			}
		}

		// Update Camera
		/* * * * * * * * * * * * * * * * * * * * */
		if (const ml::Model * target = ML_Res.models.get("earth"))
		{
			// Look
			ml::vec3f pos = target->transform().getPosition();
			ml::vec3f dir = (pos - uni.camPos).normalized();
			ml::vec3f look = uni.camPos + (pos - uni.camPos).normalized();
			uni.camera.lookAt(uni.camPos, look, ml::vec3f::Up);

			// Orbit
			float speed = (uni.camAnimate ? uni.camSpd * ev.elapsed.delta() : 0.0f);
			ml::vec3f fwd = (look - uni.camPos);
			ml::vec3f right = (fwd.cross(ml::vec3f::Up) * ml::vec3f(1, 0, 1)).normalized();
			uni.camPos += right * speed;
		}

		// Update Text
		/* * * * * * * * * * * * * * * * * * * * */
		{
			m_text["message"]
				.setFont(ML_Res.fonts.get("minecraft"))
				.setFontSize(72)
				.setPosition({ 32, 128 })
				.setString("there is no need\nto be upset");

			const ml::Font *font		= ML_Res.fonts.get("consolas");
			uint32_t		fontSize	= 18;
			float			hOff		= 0.0f;
			float			vOff		= 4.0f;
			ml::vec2f		offset		= { hOff, -(vOff + (float)fontSize) };
			ml::vec2f		origin		= { (float)fontSize, (float)this->height() - 48 };
			ml::vec2f		linePos		= 0;
			size_t			lineNum		= 0;
			auto			nextLine	= [&]() { return linePos = (origin + (offset * (float)(lineNum++))); };

			m_text["gl_version"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(nextLine())
				.setString(ml::String("GL Version: {0}").format(
					ml::OpenGL::getString(ml::GL::Version)));


			m_text["gl_vendor"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(nextLine())
				.setString(ml::String("GL Vendor: {0}").format(
					ml::OpenGL::getString(ml::GL::Vendor)));

			nextLine();

			m_text["fps_str"]
				.setFont(font)
				.setFontSize(fontSize)
				.setPosition(nextLine())
				.setString(ml::String("{0} ms/frame ({1} fps)").format(
					ev.elapsed.delta(),
					ML_Time.calculateFPS(ev.elapsed.delta())));

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

			// Update All Text
			for (auto pair : m_text) 
			{
				pair.second.update(); 
			}
		}

		// Update Network
		/* * * * * * * * * * * * * * * * * * * * */
		{
			if (SETTINGS.isServer)
			{
				ML_Server.poll();
			}
			else if (SETTINGS.isClient)
			{
				ML_Client.poll();
			}
		}

		// Update Uniforms
		/* * * * * * * * * * * * * * * * * * * * */
		{
			uni.deltaTime = ev.elapsed.delta();
			uni.totalTime = (float)ML_Time.elapsed().millis();
		}
	}

	void Demo::onDraw(const DrawEvent & ev)
	{
		// Uniforms
		/* * * * * * * * * * * * * * * * * * * * */
		static ml::UniformSet time_uniforms = {
			ml::Uniform("Time.deltaTime", ml::Uniform::Float, &uni.deltaTime),
			ml::Uniform("Time.totalTime", ml::Uniform::Float, &uni.totalTime),
		};

		static ml::UniformSet camera_uniforms = {
			ml::Uniform("Vert.view",	ml::Uniform::Mat4,	&uni.camera.matrix()),
			ml::Uniform("Vert.proj",	ml::Uniform::Mat4,	&uni.persp.matrix()),
		};

		static ml::UniformSet batch_uniforms = {
			ml::Uniform("Vert.proj",	ml::Uniform::Mat4,	&uni.ortho.matrix()),
			ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4),
			ml::Uniform("Frag.mainTex",	ml::Uniform::Tex2D),
		};

		static ml::UniformSet light_uniforms = {
			ml::Uniform("Frag.camera",		ml::Uniform::Vec3,	&uni.camPos),
			ml::Uniform("Frag.position",	ml::Uniform::Vec3,	&uni.lightPos),
			ml::Uniform("Frag.color",		ml::Uniform::Vec4,	&uni.lightCol),
			ml::Uniform("Frag.ambient",		ml::Uniform::Float, &uni.ambient),
			ml::Uniform("Frag.specular",	ml::Uniform::Float, &uni.specular),
			ml::Uniform("Frag.shininess",	ml::Uniform::Int,	&uni.shininess),
		};

		static ml::UniformSet effect_uniforms = {
			ml::Uniform("Effect.mode", ml::Uniform::Int, &uni.effectMode),
		};


		// Draw Scene
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Effect * scene = ML_Res.effects.get(ML_FBO_MAIN))
		{
			// Bind
			/* * * * * * * * * * * * * * * * * * * * */
			scene->bind();

			// Clear Screen
			/* * * * * * * * * * * * * * * * * * * * */
			this->clear(uni.clearColor);

			/* * * * * * * * * * * * * * * * * * * * */

			ml::OpenGL::enable(ml::GL::CullFace);
			ml::OpenGL::enable(ml::GL::DepthTest);

			/* * * * * * * * * * * * * * * * * * * * */

			// Light
			/* * * * * * * * * * * * * * * * * * * * */
			if (const ml::Model * model = ML_Res.models.get("light"))
			{
				if (const ml::Shader * shader = ML_Res.shaders.get("solid"))
				{
					static ml::UniformSet uniforms = {
						ml::Uniform("Vert.model",	ml::Uniform::Mat4,	&model->transform().matrix()),
						ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4,	&uni.lightCol),
					};
					shader->applyUniforms(camera_uniforms);
					shader->applyUniforms(uniforms);
					shader->bind();
					this->draw(*model);
				}
			}

			// Borg
			/* * * * * * * * * * * * * * * * * * * * */
			if (const ml::Model * model = ML_Res.models.get("borg"))
			{
				if (const ml::Shader * shader = ML_Res.shaders.get("basic"))
				{
					static ml::UniformSet uniforms = {
						ml::Uniform("Vert.model",	ml::Uniform::Mat4,	&model->transform().matrix()),
						ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4,	&ml::Color::White),
						ml::Uniform("Frag.mainTex",	ml::Uniform::Tex2D,	ML_Res.textures.get("borg")),
					};
					shader->applyUniforms(uniforms);
					shader->bind();
					this->draw(*model);
				}
			}

			// Earth
			/* * * * * * * * * * * * * * * * * * * * */
			if (const ml::Model * model = ML_Res.models.get("earth"))
			{
				if (const ml::Shader * shader = ML_Res.shaders.get("lighting"))
				{
					static ml::UniformSet uniforms = {
						ml::Uniform("Vert.model",	ml::Uniform::Mat4,	&model->transform().matrix()),
						ml::Uniform("Frag.tex_dm",	ml::Uniform::Tex2D,	ML_Res.textures.get("earth_dm")),
						ml::Uniform("Frag.tex_sm",	ml::Uniform::Tex2D,	ML_Res.textures.get("earth_sm")),
					};
					shader->applyUniforms(camera_uniforms);
					shader->applyUniforms(light_uniforms);
					shader->applyUniforms(uniforms);
					shader->bind();
					this->draw(*model);
				}
			}

			// Moon
			/* * * * * * * * * * * * * * * * * * * * */
			if (const ml::Model * model = ML_Res.models.get("moon"))
			{
				if (const ml::Shader * shader = ML_Res.shaders.get("lighting"))
				{
					static ml::UniformSet uniforms = {
						ml::Uniform("Vert.model",	ml::Uniform::Mat4,	&model->transform().matrix()),
						ml::Uniform("Frag.tex_dm",	ml::Uniform::Tex2D,	ML_Res.textures.get("moon_dm")),
						ml::Uniform("Frag.tex_sm",	ml::Uniform::Tex2D,	ML_Res.textures.get("moon_nm")),
					};
					shader->applyUniforms(camera_uniforms);
					shader->applyUniforms(light_uniforms);
					shader->applyUniforms(uniforms);
					shader->bind();
					this->draw(*model);
				}
			}

			// Cube
			/* * * * * * * * * * * * * * * * * * * * */
			if (const ml::Model * model = ML_Res.models.get("cube"))
			{
				if (const ml::Shader * shader = ML_Res.shaders.get("normal3D"))
				{
					static ml::UniformSet uniforms = {
						ml::Uniform("Vert.model",	ml::Uniform::Mat4,	&model->transform().matrix()),
						ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4,	&ml::Color::White),
						ml::Uniform("Frag.mainTex",	ml::Uniform::Tex2D,	ML_Res.textures.get("stone_dm")),
					};
					shader->applyUniforms(camera_uniforms);
					shader->applyUniforms(uniforms);
					shader->bind();
					this->draw(*model);
				}
			}

			// Ground
			/* * * * * * * * * * * * * * * * * * * * */
			if (const ml::Model * model = ML_Res.models.get("ground"))
			{
				if (const ml::Shader * shader = ML_Res.shaders.get("normal3D"))
				{
					static ml::UniformSet uniforms = {
						ml::Uniform("Vert.model",	ml::Uniform::Mat4,	&model->transform().matrix()),
						ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4,	&ml::Color::White),
						ml::Uniform("Frag.mainTex",	ml::Uniform::Tex2D,	ML_Res.textures.get("stone_dm")),
					};
					shader->applyUniforms(camera_uniforms);
					shader->applyUniforms(uniforms);
					shader->bind();
					this->draw(*model);
				}
			}

			/* * * * * * * * * * * * * * * * * * * * */

			ml::OpenGL::disable(ml::GL::CullFace);

			/* * * * * * * * * * * * * * * * * * * * */

			// Sanic
			/* * * * * * * * * * * * * * * * * * * * */
			if (const ml::Model * model = ML_Res.models.get("sanic"))
			{
				if (const ml::Shader * shader = ML_Res.shaders.get("basic"))
				{
					static ml::UniformSet uniforms = {
						ml::Uniform("Vert.model",	ml::Uniform::Mat4,	&model->transform().matrix()),
						ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4,	&ml::Color::White),
						ml::Uniform("Frag.mainTex",	ml::Uniform::Tex2D,	ML_Res.textures.get("sanic")),
					};
					shader->applyUniforms(camera_uniforms);
					shader->applyUniforms(uniforms);
					shader->bind();
					this->draw(*model);
				}
			}

			/* * * * * * * * * * * * * * * * * * * * */

			ml::OpenGL::disable(ml::GL::DepthTest);

			/* * * * * * * * * * * * * * * * * * * * */

			// Geometry
			/* * * * * * * * * * * * * * * * * * * * */
			if (const ml::Shader * shader = ML_Res.shaders.get("geometry"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4,	&uni.lineColor),
					ml::Uniform("Curve.mode",	ml::Uniform::Int,	&uni.lineMode),
					ml::Uniform("Curve.delta",	ml::Uniform::Float, &uni.lineDelta),
					ml::Uniform("Curve.size",	ml::Uniform::Float, &uni.lineSize),
					ml::Uniform("Curve.samples",ml::Uniform::Int,	&uni.lineSamples),
				};
				shader->applyUniforms(uniforms);
				shader->bind();
				ml::OpenGL::drawArrays(ml::GL::Points, 0, 4);
			}

			// Sprites
			/* * * * * * * * * * * * * * * * * * * * */
			if (const ml::Shader * shader = ML_Res.shaders.get("sprites"))
			{
				static ml::RenderBatch batch(
					&m_batchVAO, 
					&m_batchVBO, 
					shader, 
					&batch_uniforms);

				for (auto pair : ML_Res.sprites)
				{
					this->draw((*pair.second), batch);
				}
			}

			// Text
			/* * * * * * * * * * * * * * * * * * * * */
			if (const ml::Shader * shader = ML_Res.shaders.get("text"))
			{
				static ml::RenderBatch batch(
					&m_batchVAO,
					&m_batchVBO, 
					shader,
					&batch_uniforms);

				for (auto it = m_text.begin(); it != m_text.end(); it++)
				{
					this->draw(it->second, batch);
				}
			}

			// Unbind
			/* * * * * * * * * * * * * * * * * * * * */
			scene->unbind();
		}


		// Draw Effects
		/* * * * * * * * * * * * * * * * * * * * */
		if (ml::Effect * post = ML_Res.effects.get(ML_FBO_POST))
		{
			post->bind();
			
			if (ml::Effect * scene = ML_Res.effects.get(ML_FBO_MAIN))
			{
				if (const ml::Shader * shader = scene->shader())
				{
					shader->applyUniforms(effect_uniforms);

					this->draw(*scene);
				}
			}

			post->unbind();
		}


		// Draw GUI
		/* * * * * * * * * * * * * * * * * * * * */
		ImGui_ML_NewFrame();
		ImGui::NewFrame();
		{
			ML_EventSystem.fireEvent(GuiEvent(ev.elapsed));
		}
		ImGui::Render();
		ImGui_ML_Render(ImGui::GetDrawData());

		/* * * * * * * * * * * * * * * * * * * * */

		this->swapBuffers();

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Demo::onGui(const GuiEvent & ev)
	{
		// Main Menu Bar
		if (ML_MainMenuBar_draw())	{ /* Main Menu Bar */ }

		// Dockspace
		if (gui.show_dockspace)		{ ML_Dockspace_draw(&gui.show_dockspace); }

		// ImGui
		if (gui.show_imgui_demo)	{ ml::ImGui_Builtin::showDemo(&gui.show_imgui_demo); }
		if (gui.show_imgui_metrics) { ml::ImGui_Builtin::showMetrics(&gui.show_imgui_metrics); }
		if (gui.show_imgui_style)	{ ml::ImGui_Builtin::showStyle(&gui.show_imgui_style); }
		if (gui.show_imgui_about)	{ ml::ImGui_Builtin::showAbout(&gui.show_imgui_about); }

		// Editor
		if (gui.show_network)		{ ML_NetworkHUD.draw(&gui.show_network); }
		if (gui.show_profiler)		{ ML_Profiler.draw(&gui.show_profiler); }
		if (gui.show_hierarchy)		{ ML_Hierarchy.draw(&gui.show_hierarchy); }
		if (gui.show_resources)		{ ML_ResourceHUD.draw(&gui.show_resources); }
		if (gui.show_browser)		{ ML_Browser.draw(&gui.show_browser); }
		if (gui.show_terminal)		{ ML_Terminal.draw(&gui.show_terminal); }
		if (gui.show_texteditor)	{ ML_TextEditor.draw(&gui.show_texteditor); }
		if (gui.show_shaderTool)	{ ML_ShaderTool.draw(&gui.show_shaderTool); }
		if (gui.show_builder)		{ ML_Builder.draw(&gui.show_builder); }
		if (gui.show_inspector)		{ ML_Inspector.draw(&gui.show_inspector); }
		if (gui.show_scene)			{ ML_SceneView_draw(&gui.show_scene); }
		if (gui.show_demowindow)	{ ML_DemoWindow_draw(&gui.show_demowindow); }
	}

	void Demo::onUnload(const UnloadEvent & ev)
	{
		ml::Debug::log("Unloading...");

		ML_Res.cleanupAll();
	}

	void Demo::onExit(const ExitEvent & ev)
	{
		ml::Debug::log("Exiting...");

		ImGui_ML_Shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Demo::ML_MainMenuBar_draw()
	{
		return ML_MainMenuBar.drawFun([&]()
		{
			// File
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N", false, false)) {}
				if (ImGui::MenuItem("Open", "Ctrl+O", false, (ML_Browser.get_selected() && gui.show_browser)))
				{
					ML_OS.execute("open", ML_Browser.get_selected_path());
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl+S", false, false)) {}
				if (ImGui::MenuItem("Save All", "Ctrl+Shift+S", false, false)) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Quit", "Alt+F4")) { this->close(); }
				ImGui::EndMenu();
			}
			// Edit
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z", false, false)) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "Ctrl+X", false, false)) {}
				if (ImGui::MenuItem("Copy", "Ctrl+C", false, false)) {}
				if (ImGui::MenuItem("Paste", "Ctrl+V", false, false)) {}
				ImGui::EndMenu();
			}
			// Window
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem(ML_Terminal.title(), "Ctrl+Alt+T", &gui.show_terminal);
				ImGui::MenuItem(ML_Browser.title(), "Ctrl+Alt+E", &gui.show_browser);
				ImGui::MenuItem(ML_Builder.title(), "Ctrl+Alt+B", &gui.show_builder);
				ImGui::MenuItem(ML_SceneView.title(), "Ctrl+Alt+S", &gui.show_scene);
				ImGui::MenuItem(ML_Inspector.title(), "Ctrl+Alt+I", &gui.show_inspector);
				ImGui::MenuItem(ML_TextEditor.title(), NULL, &gui.show_texteditor);
				ImGui::MenuItem(ML_Hierarchy.title(), NULL, &gui.show_hierarchy);
				ImGui::MenuItem(ML_ResourceHUD.title(), NULL, &gui.show_resources);
				ImGui::MenuItem(ML_NetworkHUD.title(), NULL, &gui.show_network);
				ImGui::MenuItem(ML_ShaderTool.title(), NULL, &gui.show_shaderTool);
				ImGui::MenuItem("Demo Window",	NULL, &gui.show_demowindow);
				ImGui::EndMenu();
			}
			// Help
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Project Page"))
				{
					ML_OS.execute("open", SETTINGS.projectURL);
				}
				ImGui::Separator();
				ImGui::MenuItem("ImGui Demo", "Ctrl+H", &gui.show_imgui_demo);
				ImGui::MenuItem("ImGui Metrics", NULL, &gui.show_imgui_metrics);
				ImGui::MenuItem("ImGui Style Editor", NULL, &gui.show_imgui_style);
				ImGui::MenuItem("About Dear ImGui", NULL, &gui.show_imgui_about);
				ImGui::EndMenu();
			}
		});
	}

	bool Demo::ML_Dockspace_draw(bool * p_open)
	{
		return ML_Dockspace.drawFun(p_open, [&]() 
		{
			if (uint32_t root = ML_Dockspace.beginBuilder(ImGuiDockNodeFlags_None))
			{
				uint32_t left	= ML_Dockspace.splitNode(root, ImGuiDir_Left, 0.29f, &root);
				uint32_t center = ML_Dockspace.splitNode(root, ImGuiDir_Right, 0.5f, &root);
				uint32_t right	= ML_Dockspace.splitNode(center, ImGuiDir_Right, 0.21f, &center);

				const uint32_t left_U	= ML_Dockspace.splitNode(left, ImGuiDir_Up, 0.65f, &left);
				const uint32_t left_D	= ML_Dockspace.splitNode(left, ImGuiDir_Down, 0.35f, &left);
				const uint32_t center_U = ML_Dockspace.splitNode(center, ImGuiDir_Up, 0.65f, &center);
				const uint32_t center_D = ML_Dockspace.splitNode(center, ImGuiDir_Down, 0.35f, &center);
				const uint32_t right_U	= ML_Dockspace.splitNode(right, ImGuiDir_Up, 0.65f, &right);
				const uint32_t right_D	= ML_Dockspace.splitNode(right, ImGuiDir_Down, 0.35f, &right);

				ML_Dockspace.dockWindow(ML_Profiler.title(),	left_U);
				ML_Dockspace.dockWindow(ML_NetworkHUD.title(),	left_U);
				ML_Dockspace.dockWindow(ML_Browser.title(),		left_U);
				ML_Dockspace.dockWindow(ML_Hierarchy.title(),	left_U);
				ML_Dockspace.dockWindow(ML_ResourceHUD.title(), left_U);
				ML_Dockspace.dockWindow(ML_Terminal.title(),	left_D);
				ML_Dockspace.dockWindow(ML_SceneView.title(),	center_U);
				ML_Dockspace.dockWindow(ML_Builder.title(),		center_D);
				ML_Dockspace.dockWindow(ML_TextEditor.title(),	center_D);
				ML_Dockspace.dockWindow(ML_ShaderTool.title(),	center_D);
				ML_Dockspace.dockWindow(ML_Inspector.title(),	right_D);
				ML_Dockspace.dockWindow("Demo Window",			right_U);

				ML_Dockspace.endBuilder(root);
			};
		});
	}

	bool Demo::ML_SceneView_draw(bool * p_open)
	{
		return ML_SceneView.drawFun(p_open, [&]()
		{
			// Update Scene Resolution
			/* * * * * * * * * * * * * * * * * * * * */
			const ml::vec2i resolution = this->getFramebufferSize();
			if (resolution != ml::vec2i::Zero)
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
					uni.ortho.orthographic(
						{ ml::vec2f::Zero, (ml::vec2f)resolution },
						{ SETTINGS.orthoNear, SETTINGS.orthoFar }
					);

					// Perspective
					uni.persp.perspective(
						SETTINGS.fieldOfView, ((float)resolution[0] / (float)resolution[1]),
						SETTINGS.perspNear, SETTINGS.perspFar
					);
				}
			}

			// Draw Scene
			/* * * * * * * * * * * * * * * * * * * * */
			if (ml::Effect * e = ML_Res.effects.get(ML_FBO_POST))
			{
				ML_SceneView.updateTexture(e->texture());
			}
		});
	}

	bool Demo::ML_DemoWindow_draw(bool * p_open)
	{
		return ml::GUI::DrawWindow("Demo Window", p_open, ImGuiWindowFlags_None, [&]()
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::Button("Reload Shaders"))
			{
				ml::Debug::log("Reloaded {0} Shaders.", ML_Res.shaders.reload());
			}
			ImGui::Separator();

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::Text("Scene");
			ImGui::ColorEdit4("Color##Scene", &uni.clearColor[0]);
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
			ImGui::Combo("Shader##Framebuffer", &uni.effectMode, fbo_modes, IM_ARRAYSIZE(fbo_modes));
			ImGui::Separator();

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::Text("Camera");
			ImGui::Checkbox("Move##Camera", &uni.camAnimate);
			ml::GUI::EditVec3f("Position##Camera", uni.camPos);
			ImGui::DragFloat("Speed##Camera", &uni.camSpd, 0.1f, -5.f, 5.f);
			ImGui::Separator();

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::Text("Light");
			ml::GUI::EditVec3f("Position##Light", uni.lightPos);
			ImGui::ColorEdit4("Color##Light", &uni.lightCol[0]);
			ImGui::DragFloat("Ambient##Light", &uni.ambient, 0.01f, 0.f, 1.f);
			ImGui::DragFloat("Specular##Light", &uni.specular, 0.01f, 0.1f, 10.f);
			ImGui::DragInt("Shininess##Light", &uni.shininess, 1.f, 1, 256);
			ImGui::Separator();

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui::Text("Geometry");
			ImGui::SliderInt("Mode##Geometry", &uni.lineMode, -1, 3);
			ImGui::ColorEdit4("Color##Geometry", &uni.lineColor[0]);
			ImGui::SliderFloat("Delta##Geometry", &uni.lineDelta, 0.f, 1.f);
			ImGui::SliderFloat("Size##Geometry", &uni.lineSize, 0.f, 1.f);
			ImGui::SliderInt("Samples##Geometry", &uni.lineSamples, 1, 128);
			ImGui::Separator();

			/* * * * * * * * * * * * * * * * * * * * */

			if (ml::Model * m = ML_Res.models.get("earth"))
			{
				ImGui::Text("Transform");
				ImGui::Checkbox("Animate##Transform", &uni.animate);
				ml::GUI::EditMat4f("Matrix", m->transform().matrix());
				ImGui::Separator();
			}

			/* * * * * * * * * * * * * * * * * * * * */
		});
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */