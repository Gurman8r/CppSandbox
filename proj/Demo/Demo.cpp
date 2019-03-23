#include "Demo.hpp"
#include "DemoCommands.hpp"
#include <MemeCore/EventSystem.hpp>
#include <MemeCore/OS.hpp>
#include <MemeWindow/WindowEvents.hpp>
#include <MemeEditor/ResourceManager.hpp>
#include <MemeEditor/EditorGUI.hpp>
#include <MemeEditor/Terminal.hpp>
#include <MemeEditor/Builder.hpp>
#include <MemeEditor/Browser.hpp>
#include <MemeEditor/ImGui_Builtin.hpp>
#include <MemeEditor/Dockspace.hpp>
#include <MemeEditor/TextEditor.hpp>
#include <MemeEditor/ImGui.hpp>

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
		ML_EventSystem.addListener(DemoEvent::EV_Exit, this);

		ML_EventSystem.addListener(ml::CoreEvent::EV_RequestExit, this);
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
		case DemoEvent::EV_Exit:		return onExit(*value->as<ExitEvent>());

		case ml::CoreEvent::EV_RequestExit:
			if (const auto * ev = value->as<ml::RequestExitEvent>())
				this->close();
			break;

		case ml::WindowEvent::EV_FramebufferSize:
			if (const auto * ev = value->as<ml::FramebufferSizeEvent>())
			{
				this->setViewport(ml::vec2i::Zero, ev->size());

				// Orthographic
				m_ortho.orthographic(
					{ ml::vec2f::Zero, (ml::vec2f)ev->size() },
					{ SETTINGS.orthoNear, SETTINGS.orthoFar }
				);

				// Perspective
				m_persp.perspective(
					SETTINGS.fieldOfView, ev->aspect(),
					SETTINGS.perspNear, SETTINGS.perspFar
				);

				// Framebuffers
				if (ev->size() != ml::vec2i::Zero)
				{
					auto resizeFBO = [](ml::Texture * tex, auto size) 
					{
						tex->cleanup();
						tex->create(size);
					};

					//resizeFBO(ML_Res.textures.get("framebuffer"), ev->size());
					//resizeFBO(ML_Res.textures.get("post"), ev->size());

					if (ml::Texture * tex = ML_Res.textures.get("framebuffer"))
					{
						tex->cleanup();
						tex->create(ev->size());
						m_effects["default"].reload(ev->size());
					}

					if (ml::Texture * tex = ML_Res.textures.get("post"))
					{
						tex->cleanup();
						tex->create(ev->size());
						m_effects["post"].reload(ev->size());
					}
				}
			}
			break;

		case ml::WindowEvent::EV_Key:
			if (const auto * ev = value->as<ml::KeyEvent>())
			{
				/* * * * * * * * * * * * * * * * * * * * */

				// Mods
				const bool mod_ctrl  = (ev->mods & ML_MOD_CTRL);
				const bool mod_alt	 = (ev->mods & ML_MOD_ALT);
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
				if (ev->getKeyDown(ml::KeyCode::T) && (mod_ctrl && mod_alt)) { show_ml_terminal = true; }
				
				// Browser (Ctrl+Alt+E)
				if (ev->getKeyDown(ml::KeyCode::E) && (mod_ctrl)) { show_ml_browser = true; }
				
				// Builder (Ctrl+Alt+B)
				if (ev->getKeyDown(ml::KeyCode::B) && (mod_ctrl && mod_alt)) { show_ml_builder = true; }

				// Scene (Ctrl+Alt+S)
				if (ev->getKeyDown(ml::KeyCode::S) && (mod_ctrl && mod_alt)) { show_ml_scene = true; }

				// Inspector (Ctrl+Alt+I)
				if (ev->getKeyDown(ml::KeyCode::I) && (mod_ctrl && mod_alt)) { show_ml_inspector = true; }
				
				// ImGui Demo (Ctrl+H)
				if (ev->getKeyDown(ml::KeyCode::H) && (mod_ctrl)) { show_imgui_demo = true; }

				/* * * * * * * * * * * * * * * * * * * * */
			}
			break;
		}

	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Demo::loadResources()
	{
		ml::ResourceManifest manifest;
		if (manifest.loadFromFile(ML_FileSystem.pathTo(SETTINGS.pathTo(SETTINGS.manifest))))
		{
			return ml::Debug::log("Loading Resources...")
				&& ML_Res.meshes.load("default_quad")->loadFromMemory(ml::Shapes::Quad::Vertices, ml::Shapes::Quad::Indices)
				&& ML_Res.meshes.load("default_cube")->loadFromMemory(ml::Shapes::Cube::Vertices, ml::Shapes::Cube::Indices)
				&& ML_Res.models.load("borg")->loadFromMemory(*ML_Res.meshes.get("default_cube"))
				&& ML_Res.models.load("sanic")->loadFromMemory(*ML_Res.meshes.get("default_quad"))
				&& ML_Res.models.load("sprite")->loadFromMemory(*ML_Res.meshes.get("default_quad"))
				&& ML_Res.models.load("framebuffer")->loadFromMemory(*ML_Res.meshes.get("default_quad"))
				&& ML_Res.textures.load("framebuffer")->create(this->getFramebufferSize())
				&& ML_Res.textures.load("post")->create(this->getFramebufferSize())
				&& ML_Res.loadManifest(manifest)
				&& loadBuffers();
		}
		return ml::Debug::logError("Failed loading manifest");
	}

	bool Demo::loadBuffers()
	{
		// Batch
		m_vao.create(ml::GL::Triangles).bind();
		m_vbo.create(ml::GL::DynamicDraw).bind();
		m_vbo.bufferData(NULL, ml::RectQuad::Size);
		ml::BufferLayout::Default.bind();
		m_vbo.unbind();
		m_vao.unbind();

		// Effects
		m_effects["default"].create(this->getSize(), ml::GL::ColorAttachment0);
		m_effects["default"].setModel(ML_Res.models.get("framebuffer"));
		m_effects["default"].setShader(ML_Res.shaders.get("framebuffer"));
		m_effects["default"].setTexture(ML_Res.textures.get("framebuffer"));

		m_effects["post"].create(this->getSize(), ml::GL::ColorAttachment0);
		m_effects["post"].setModel(ML_Res.models.get("framebuffer"));
		m_effects["post"].setShader(ML_Res.shaders.get("framebuffer"));
		m_effects["post"].setTexture(ML_Res.textures.get("post"));

		return true;
	}

	bool Demo::loadNetwork()
	{
		if ((SETTINGS.isServer || SETTINGS.isClient) &&
			ml::Debug::log("Loading Network..."))
		{
			if (SETTINGS.isServer)
			{
				// Server Setup
				if (ML_Server.setup())
				{
					if (ML_Server.start({ ML_LOCALHOST, ML_PORT }, ML_MAX_CLIENTS))
					{
						return ml::Debug::log("Server Started: {0}", ML_Server.getMyAddress());
					}
				}
			}
			else if (SETTINGS.isClient)
			{
				// Client Setup
				if (ML_Client.setup())
				{
					if (ML_Client.connect({ ML_LOCALHOST, ML_PORT }))
					{
						return ml::Debug::log("Client Connected: ");
					}
				}
			}
		}
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Demo::onEnter(const EnterEvent & ev)
	{
		// Seed Random
		ml::Random::seed();

		// Start Master Timer
		ML_Time.start();

		// Set Parser Flags
		ML_Parser.showToks(SETTINGS.scrShowToks); // Show Tokens
		ML_Parser.showTree(SETTINGS.scrShowTree); // Show Tree
		ML_Parser.showItoP(SETTINGS.scrShowItoP); // Show Infix to Postfix

		// Load Commands
		install_commands();

		// Run Script
		if (!SETTINGS.scrFile.empty())
		{
			ml::Script scr;
			if (scr.loadFromFile(SETTINGS.pathTo(SETTINGS.scrPath + SETTINGS.scrFile)))
			{
				if (scr.build(ev.args))
				{
					if (scr.run())
					{
						if (scr.retv().isErrorType())
						{
							ml::Debug::setError(ml::Debug::logError("Script returned an error"));
						}
					}
					else
					{
						ml::Debug::setError(ml::Debug::logError("Failed running script"));
					}
				}
				else
				{
					ml::Debug::setError(ml::Debug::logError("Failed building script"));
				}
			}
			else
			{
				ml::Debug::setError(ml::Debug::logError("Failed loading script"));
			}
		}
		else
		{
			ml::Debug::setError(ml::Debug::logWarning("No Script"));
		}
	}

	void Demo::onLoad(const LoadEvent & ev)
	{
		ml::OpenGL::errorPause(SETTINGS.glErrorPause);

		if (this->create(SETTINGS.title, SETTINGS.video(), SETTINGS.style, SETTINGS.context()) && 
			this->setup())
		{
			this->setInputMode(ml::Cursor::Normal);
			this->setPosition((ml::VideoMode::desktop().size - this->getSize()) / 2);
			this->setViewport(ml::vec2i::Zero, this->getFramebufferSize());
			this->maximize();

			// Load ImGui
			if (ml::Debug::log("Dear ImGui..."))
			{
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGui::StyleColorsDark();
				ImGui::GetStyle().FrameBorderSize = 1;
				ml::Debug::log("ImGui Ini: \"{0}\"", SETTINGS.imguiIni.c_str());
				if (!ImGui_ML_Init("#version 410", this, true, SETTINGS.imguiIni.c_str()))
				{
					return ml::Debug::setError(ml::Debug::logError("Failed Loading ImGui"));
				}
			}

			// Load OpenAL
			if (!ml::OpenAL::init())
			{
				return ml::Debug::setError(ml::Debug::logError("Failed Loading OpenAL"));
			}

			// Load Resources
			if (!loadResources())
			{
				return ml::Debug::setError(ml::Debug::logError("Failed Loading Resources"));
			}

			// Load Network
			if (!loadNetwork())
			{
				return ml::Debug::setError(ml::Debug::logError("Failed Loading Network"));
			}

			// Load Plugins
			if (m_plugin.loadFromFile(ml::String("./{0}_{1}_{2}.dll").format(
				(SETTINGS.pluginName),
				(ml::Debug::config()),
				(ml::Debug::platform()))))
			{
				m_plugin.main((void *)"Plugin Received Data");
				if (m_plugin.cleanup())
				{
					ml::Debug::log("Plugin Finalized");
				}
			}
			else
			{
				ml::Debug::setError(ml::Debug::logError("Failed Loading Plugin"));
			}
		}
		else
		{
			return ml::Debug::setError(ml::Debug::logError(
				"Failed Loading Window\n"
				"Try checking your settings:\n"
				"\"{0}\"\n",
				SETTINGS.iniPath));
		}
	}

	void Demo::onStart(const StartEvent & ev)
	{
		ml::Debug::log("Starting...");

		// Set Window Icon
		if (ml::Image * icon = ML_Res.images.get("icon"))
		{
			this->setIcons({ (*icon).flipVertically() });
		}

		// Setup Projections
		{
			// Orthographic
			m_ortho.orthographic(
				{ ml::vec2f::Zero, (ml::vec2f)this->getSize() },
				{ SETTINGS.orthoNear, SETTINGS.orthoFar }
			);

			// Perspective
			m_persp.perspective(
				SETTINGS.fieldOfView, this->aspect(),
				SETTINGS.perspNear, SETTINGS.perspFar
			);
		}

		// Setup Camera
		{
			m_camera.lookAt(m_camPos, m_camPos + ml::vec3f::Back, ml::vec3f::Up);
		}

		// Setup Models
		{
			ML_Res.models.get("borg")->transform()
				.translate({ +5.0f, 0.0f, 0.0f });

			ML_Res.models.get("sanic")->transform()
				.translate({ -5.0f, 0.0f, 0.0f });

			ML_Res.models.get("earth")->transform()
				.translate({ 0.0f, 0.0f, 0.0f });

			ML_Res.models.get("cube")->transform()
				.translate({ 0.0f, 0.0f, -5.0f })
				.scale(0.5f);

			ML_Res.models.get("moon")->transform()
				.translate({ 0.0f, 0.0f, 5.0f })
				.scale(0.5f);

			ML_Res.models.get("ground")->transform()
				.translate({ 0.0f, -2.5f, 0.0f })
				.scale({ 12.5, 0.25f, 12.5 });
		}

		// Setup Threads
		m_thread = new ml::Thread([&]()
		{
			ml::Debug::log("Entering Dummy Thread");
			do
			{
				ml::cout << "*";
				ML_Time.sleep(1000);
			} while (this->isOpen());
			ml::cout << ml::endl;
			ml::Debug::log("Exiting Dummy Thread");
		});
		if (SETTINGS.enableThreads) { m_thread->launch(); }
	}

	void Demo::onFixedUpdate(const FixedUpdateEvent & ev)
	{
		m_particle.applyForce(ml::Force::gravity(ml::vec3f::Up, m_particle.mass));
	}

	void Demo::onUpdate(const UpdateEvent & ev)
	{
		// Poll Events
		this->pollEvents();

		// Update Title
		this->setTitle(ml::String::Format("{0} | {1} | {2}",
			SETTINGS.title,
			ml::String("{0} | {1}").format(
				ml::Debug::config(),
				ml::Debug::platform()
			),
			ml::String("{0} ms/frame ({1} fps)").format(
				ev.elapsed.delta(),
				ML_Time.calculateFPS(ev.elapsed.delta())
			)
		));

		// Update Models
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
					.rotate((m_animate ? ev.elapsed.delta() : 0.f), ml::vec3f::Up);
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
					.setPosition(m_lightPos)
					.rotate(-ev.elapsed.delta(), ml::vec3f::Forward);
			}
		}

		// Update Camera
		if (const ml::Model * target = ML_Res.models.get("earth"))
		{
			// Look
			ml::vec3f pos = target->transform().getPosition();
			ml::vec3f dir = (pos - m_camPos).normalized();
			ml::vec3f look = m_camPos + (pos - m_camPos).normalized();
			m_camera.lookAt(m_camPos, look, ml::vec3f::Up);

			// Orbit
			float speed = (m_camAnimate ? m_camSpd * ev.elapsed.delta() : 0.0f);
			ml::vec3f fwd = (look - m_camPos);
			ml::vec3f right = (fwd.cross(ml::vec3f::Up) * ml::vec3f(1, 0, 1)).normalized();
			m_camPos += right * speed;
		}

		// Update Text
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

			// Update All
			for (auto pair : m_text) { pair.second.update(); }
		}

		// Update Network
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
	}

	void Demo::onDraw(const DrawEvent & ev)
	{
		// Uniforms
		/* * * * * * * * * * * * * * * * * * * * */
		static ml::UniformSet camera_uniforms = {
			ml::Uniform("Vert.view",	ml::Uniform::Mat4,	&m_camera.matrix()),
			ml::Uniform("Vert.proj",	ml::Uniform::Mat4,	&m_persp.matrix()),
		};

		static ml::UniformSet batch_uniforms = {
			ml::Uniform("Vert.proj",	ml::Uniform::Mat4,	&m_ortho.matrix()),
			ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4),
			ml::Uniform("Frag.mainTex",	ml::Uniform::Tex),
		};

		static ml::UniformSet light_uniforms = {
			ml::Uniform("Frag.camera",		ml::Uniform::Vec3,	&m_camPos),
			ml::Uniform("Frag.position",	ml::Uniform::Vec3,	&m_lightPos),
			ml::Uniform("Frag.color",		ml::Uniform::Vec4,	&m_lightCol),
			ml::Uniform("Frag.ambient",		ml::Uniform::Float, &m_ambient),
			ml::Uniform("Frag.specular",	ml::Uniform::Float, &m_specular),
			ml::Uniform("Frag.shininess",	ml::Uniform::Int,	&m_shininess),
		};

		static ml::UniformSet effect_uniforms = {
			ml::Uniform("Effect.mode", ml::Uniform::Int, &m_effectMode),
		};

		// Draw Scene Objects
		/* * * * * * * * * * * * * * * * * * * * */
		m_effects["default"].bind();
		{
			// Clear
			this->clear(m_clearColor);

			ml::OpenGL::enable(ml::GL::CullFace);
			ml::OpenGL::enable(ml::GL::DepthTest);

			// Light
			if (const ml::Model * model = ML_Res.models.get("light"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("Vert.model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4,	&m_lightCol),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("solid"))
				{
					shader->applyUniforms(camera_uniforms);
					shader->applyUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Borg
			if (const ml::Model * model = ML_Res.models.get("borg"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("Vert.model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4,	&ml::Color::White),
					ml::Uniform("Frag.mainTex",	ml::Uniform::Tex,	ML_Res.textures.get("borg")),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("basic"))
				{
					shader->applyUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Earth
			if (const ml::Model * model = ML_Res.models.get("earth"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("Vert.model",		ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("Frag.tex_dm",		ml::Uniform::Tex,	ML_Res.textures.get("earth_dm")),
					ml::Uniform("Frag.tex_sm",		ml::Uniform::Tex,	ML_Res.textures.get("earth_sm")),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("lighting"))
				{
					shader->applyUniforms(camera_uniforms);
					shader->applyUniforms(light_uniforms);
					shader->applyUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Moon
			if (const ml::Model * model = ML_Res.models.get("moon"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("Vert.model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("Frag.tex_dm",	ml::Uniform::Tex,	ML_Res.textures.get("moon_dm")),
					ml::Uniform("Frag.tex_sm",	ml::Uniform::Tex,	ML_Res.textures.get("moon_nm")),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("lighting"))
				{
					shader->applyUniforms(camera_uniforms);
					shader->applyUniforms(light_uniforms);
					shader->applyUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Cube
			if (const ml::Model * model = ML_Res.models.get("cube"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("Vert.model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4,	&ml::Color::White),
					ml::Uniform("Frag.mainTex",	ml::Uniform::Tex,	ML_Res.textures.get("stone_dm")),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("normal3D"))
				{
					shader->applyUniforms(camera_uniforms);
					shader->applyUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Ground
			if (const ml::Model * model = ML_Res.models.get("ground"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("Vert.model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4,	&ml::Color::White),
					ml::Uniform("Frag.mainTex",	ml::Uniform::Tex,	ML_Res.textures.get("stone_dm")),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("normal3D"))
				{
					shader->applyUniforms(camera_uniforms);
					shader->applyUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			ml::OpenGL::disable(ml::GL::CullFace);

			// Sanic
			if (const ml::Model * model = ML_Res.models.get("sanic"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("Vert.model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4,	&ml::Color::White),
					ml::Uniform("Frag.mainTex",	ml::Uniform::Tex,	ML_Res.textures.get("sanic")),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("basic"))
				{
					shader->applyUniforms(camera_uniforms);
					shader->applyUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			ml::OpenGL::disable(ml::GL::DepthTest);

			// Geometry
			if (const ml::Shader * shader = ML_Res.shaders.get("geometry"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("Frag.mainCol",	ml::Uniform::Vec4,	&m_lineColor),
					ml::Uniform("Curve.mode",	ml::Uniform::Int,	&m_lineMode),
					ml::Uniform("Curve.delta",	ml::Uniform::Float, &m_lineDelta),
					ml::Uniform("Curve.size",	ml::Uniform::Float, &m_lineSize),
					ml::Uniform("Curve.samples",ml::Uniform::Int,	&m_lineSamples),
				};
				shader->applyUniforms(uniforms);
				shader->bind();
				ml::OpenGL::drawArrays(ml::GL::Points, 0, 4);
			}

			// Sprites
			if (const ml::Shader * shader = ML_Res.shaders.get("sprites"))
			{
				static ml::RenderBatch batch(&m_vao, &m_vbo, shader, &batch_uniforms);

				static ml::Sprite sprite;
				sprite
					.setTexture(ML_Res.textures.get("neutrino"))
					.setPosition((ml::vec2f{ 0.95f, 0.075f } * this->getSize()))
					.setScale(0.5f)
					.setRotation(0.0f)
					.setOrigin(0.5f)
					.setColor(ml::Color::White);

				this->draw(sprite, batch);
			}

			// Text
			if (const ml::Shader * shader = ML_Res.shaders.get("text"))
			{
				static ml::RenderBatch batch(&m_vao, &m_vbo, shader, &batch_uniforms);

				TextMap::const_iterator it;
				for (it = m_text.begin(); it != m_text.end(); it++)
				{
					this->draw(it->second, batch);
				}
			}
		}
		m_effects["default"].unbind();

		// Draw Effects
		/* * * * * * * * * * * * * * * * * * * * */
		m_effects["post"].bind();
		{
			m_effects["default"].shader()->applyUniforms(effect_uniforms);
			this->draw(m_effects["default"]);
		}
		m_effects["post"].unbind();

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
		/* * * * * * * * * * * * * * * * * * * * */
		
		// Main Menu Bar
		draw_MainMenuBar();

		// Dockspace
		if (show_ml_dockspace)	{ draw_Dockspace(&show_ml_dockspace); }

		// ImGui
		if (show_imgui_demo)	{ ml::ImGuiBuiltin::showDemoWindow(show_imgui_demo); }
		if (show_imgui_metrics) { ml::ImGuiBuiltin::showMetricsWindow(show_imgui_metrics); }
		if (show_imgui_style)	{ ml::ImGuiBuiltin::showStyleWindow(show_imgui_style); }
		if (show_imgui_about)	{ ml::ImGuiBuiltin::showAboutWindow(show_imgui_about); }

		// Editor
		if (show_ml_browser)	{ ML_Browser.draw("Browser", &show_ml_browser); }
		if (show_ml_terminal)	{ ML_Terminal.draw("Terminal", &show_ml_terminal); }
		if (show_ml_builder)	{ ML_Builder.draw("Builder", &show_ml_builder); }
		if (show_ml_scene)		{ draw_Scene(&show_ml_scene); }
		if (show_ml_inspector)	{ draw_Inspector(&show_ml_inspector); }

		static ml::TextEditor textEditor("Sample Text");
		if (show_ml_text_editor)
		{
			textEditor.draw("Text Editor", &show_ml_text_editor);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Demo::onExit(const ExitEvent & ev)
	{
		delete m_thread;

		ML_Res.cleanAll();

		ImGui_ML_Shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Demo::draw_MainMenuBar()
	{
		/* * * * * * * * * * * * * * * * * * * * */

		if (ImGui::BeginMainMenuBar())
		{
			// File
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N", false, false)) {}
				if (ImGui::MenuItem("Open", "Ctrl+O", false, (ML_Browser.get_selected() && show_ml_browser)))
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
				ImGui::MenuItem("Terminal", "Ctrl+Alt+T", &show_ml_terminal);
				ImGui::MenuItem("Browser", "Ctrl+Alt+E", &show_ml_browser);
				ImGui::MenuItem("Builder", "Ctrl+Alt+B", &show_ml_builder);
				ImGui::MenuItem("Scene", "Ctrl+Alt+S", &show_ml_scene);
				ImGui::MenuItem("Inspector", "Ctrl+Alt+I", &show_ml_inspector);
				ImGui::MenuItem("Text Editor", NULL, &show_ml_text_editor);
				ImGui::EndMenu();
			}
			// Help
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Project Page"))
				{
					ML_OS.execute("open", "https://www.github.com/Gurman8r/Cppsandbox");
				}
				ImGui::Separator();
				ImGui::MenuItem("ImGui Demo", "Ctrl+H", &show_imgui_demo);
				ImGui::MenuItem("ImGui Metrics", NULL, &show_imgui_metrics);
				ImGui::MenuItem("Style Editor", NULL, &show_imgui_style);
				ImGui::MenuItem("About Dear ImGui", NULL, &show_imgui_about);
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Demo::draw_Dockspace(bool * p_open)
	{
		/* * * * * * * * * * * * * * * * * * * * */
		
		static ml::Dockspace d("EditorDockspace", ImGuiDockNodeFlags_PassthruDockspace);

		/* * * * * * * * * * * * * * * * * * * * */
		
		auto setup_dockspace = [](uint32_t root)
		{
			if (!ImGui::DockBuilderGetNode(root)) 
			{
				ImGui::DockBuilderRemoveNode(root);
				ImGui::DockBuilderAddNode(root, ImGuiDockNodeFlags_None);

				uint32_t left = d.split(root, ImGuiDir_Left, 0.29f, &root);
				uint32_t center = d.split(root, ImGuiDir_Right, 0.5f, &root);
				uint32_t right = d.split(center, ImGuiDir_Right, 0.21f, &center);

				const uint32_t left_U = d.split(left, ImGuiDir_Up, 0.65f, &left);
				const uint32_t left_D = d.split(left, ImGuiDir_Down, 0.35f, &left);
				
				const uint32_t center_U = d.split(center, ImGuiDir_Up, 0.65f, &center);
				const uint32_t center_D = d.split(center, ImGuiDir_Down, 0.35f, &center);
				
				//const uint32_t right_U = d.split(right, ImGuiDir_Up, 0.5f, &right);
				//const uint32_t right_D = d.split(right, ImGuiDir_Down, 0.5f, &right);

				d.dock_window("Browser", left_U);
				d.dock_window("Terminal", left_D);
				d.dock_window("Scene", center_U);
				d.dock_window("Builder", center_D);
				d.dock_window("Text Editor", center_D);
				d.dock_window("Inspector", right);

				ImGui::DockBuilderFinish(root);
			}
		};
		
		/* * * * * * * * * * * * * * * * * * * * */

		if (d.begin_dock("Dockspace", p_open))
		{
			setup_dockspace(d.getID());

			d.end_dock();
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Demo::draw_Inspector(bool * p_open)
	{
		if (!ImGui::Begin("Inspector", p_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			return ImGui::End();
		}
		else
		{
			ImGui::Separator();

			if (ImGui::Button("Reload Shaders"))
			{
				ml::Debug::log("Reloaded {0} Shaders.", ML_Res.shaders.reload());
			}
			ImGui::Separator();

			ImGui::Text("Scene");
			ImGui::ColorEdit4("Color##Scene", &m_clearColor[0]);
			ImGui::Separator();

			ImGui::Text("Framebuffer");
			static ml::CString fbo_modes[] = {
					"Normal",
					"Grayscale",
					"Blur",
					"Juicy",
					"Inverted",
			};
			ImGui::Combo("Shader##Framebuffer", &m_effectMode, fbo_modes, IM_ARRAYSIZE(fbo_modes));
			ImGui::Separator();

			ImGui::Text("Camera");
			ImGui::Checkbox("Move##Camera", &m_camAnimate);
			ml::EditorGUI::InputVec3f("Position##Camera", m_camPos);
			ImGui::DragFloat("Speed##Camera", &m_camSpd, 0.1f, -5.f, 5.f);
			ImGui::Separator();

			ImGui::Text("Light");
			ml::EditorGUI::InputVec3f("Position##Light", m_lightPos);
			ImGui::ColorEdit4("Color##Light", &m_lightCol[0]);
			ImGui::DragFloat("Ambient##Light", &m_ambient, 0.01f, 0.f, 1.f);
			ImGui::DragFloat("Specular##Light", &m_specular, 0.01f, 0.1f, 10.f);
			ImGui::DragInt("Shininess##Light", &m_shininess, 1.f, 1, 256);
			ImGui::Separator();

			ImGui::Text("Geometry");
			ImGui::SliderInt("Mode##Geometry", &m_lineMode, -1, 3);
			ImGui::ColorEdit4("Color##Geometry", &m_lineColor[0]);
			ImGui::SliderFloat("Delta##Geometry", &m_lineDelta, 0.f, 1.f);
			ImGui::SliderFloat("Size##Geometry", &m_lineSize, 0.f, 1.f);
			ImGui::SliderInt("Samples##Geometry", &m_lineSamples, 1, 128);
			ImGui::Separator();

			ImGui::Text("Transform");
			ImGui::Checkbox("Animate", &m_animate);
			ml::Transform & temp = ML_Res.models.get("earth")->transform();
			
			ml::EditorGUI::InputMat4f("Matrix", temp.matrix());
			ImGui::Separator();

			return ImGui::End();
		}
	}

	void Demo::draw_Scene(bool * p_open)
	{
		static bool preserve_aspect = true;

		if (ImGui::Begin("Scene", p_open, ImGuiWindowFlags_MenuBar))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				ImGui::Checkbox("Preserve Aspect", &preserve_aspect);
				ImGui::EndMenuBar();
			}

			ImGui::BeginChild("SceneView", { -1, -1 }, false);
			{
				if (auto tex = std::remove_cv_t<ml::Texture *>(m_effects["post"].texture()))
				{
					// Texture Size
					const ml::vec2f src = tex->size();

					// Window Size
					const ml::vec2f dst = ml::vec2f { 
						ImGui::GetWindowSize().x,
						ImGui::GetWindowSize().y
					};

					auto scaleToFit = [](const ml::vec2f & src, const ml::vec2f & dst)
					{
						const ml::vec2f
							hs = (dst[0] / src[0]),
							vs = (dst[1] / src[1]);
						return (src * (((hs) < (vs)) ? (hs) : (vs)));
					};

					const ml::vec2f scl = (preserve_aspect
						? scaleToFit(src, dst)
						: (dst * 0.975f));

					const ml::vec2f pos = ((dst - scl) * 0.5f);

					ImGui::SetCursorPos({ pos[0], pos[1] });

					ImGui::Image(tex->get_raw(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });
				}
			}
			ImGui::EndChild();

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return ImGui::End();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}