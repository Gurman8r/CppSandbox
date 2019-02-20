#include "Demo.hpp"
#include <MemeEditor/InterpreterConsole.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeWindow/WindowEvents.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_ml.hpp>

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * */

	Demo::Demo()
	{
		ML_EventSystem.addListener(DemoEvent::EV_Enter,			this);
		ML_EventSystem.addListener(DemoEvent::EV_Load,			this);
		ML_EventSystem.addListener(DemoEvent::EV_Start,			this);
		ML_EventSystem.addListener(DemoEvent::EV_FixedUpdate,	this);
		ML_EventSystem.addListener(DemoEvent::EV_Update,		this);
		ML_EventSystem.addListener(DemoEvent::EV_Draw,			this);
		ML_EventSystem.addListener(DemoEvent::EV_Gui,			this);
		ML_EventSystem.addListener(DemoEvent::EV_Exit,			this);
	}

	Demo::~Demo() {}

	/* * * * * * * * * * * * * * * * * * * * */

	void Demo::onEvent(const ml::Event * value)
	{
		ml::RenderWindow::onEvent(value);

		switch (value->eventID())
		{
		case DemoEvent::EV_Enter:		return onEnter(*value->Cast<EnterEvent>());
		case DemoEvent::EV_Load:		return onLoad(*value->Cast<LoadEvent>());
		case DemoEvent::EV_Start:		return onStart(*value->Cast<StartEvent>());
		case DemoEvent::EV_FixedUpdate:	return onFixedUpdate(*value->Cast<FixedUpdateEvent>());
		case DemoEvent::EV_Update:		return onUpdate(*value->Cast<UpdateEvent>());
		case DemoEvent::EV_Draw:		return onDraw(*value->Cast<DrawEvent>());
		case DemoEvent::EV_Gui:			return onGui(*value->Cast<GuiEvent>());
		case DemoEvent::EV_Exit:		return onExit(*value->Cast<ExitEvent>());

		case ml::WindowEvent::EV_WindowSize:
			if (auto ev = value->Cast<ml::WindowSizeEvent>())
			{
				// Orthographic
				m_ortho = ml::Transform::Orthographic(
					{ ml::vec2f::Zero, (ml::vec2f)this->getFramebufferSize() },
					{ SETTINGS.orthoNear, SETTINGS.orthoFar }
				);

				// Perspective
				m_persp = ml::Transform::Perspective(
					SETTINGS.fieldOfView,
					this->aspect(),
					{ SETTINGS.perspNear, SETTINGS.perspFar }
				);
			}
			break;

		case ml::WindowEvent::EV_Key:
			if (auto ev = value->Cast<ml::KeyEvent>())
			{
				// Toggle Smooth Textures
				if (ev->getKeyDown(ml::KeyCode::Num1))
				{
					static bool smooth = false;
					smooth = !smooth;
					for (auto pair : ML_Res.textures.getAll())
					{
						pair.second->setSmooth(smooth);
					}
				}

				// Reload Shaders
				if (ev->getKeyDown(ml::KeyCode::R))
				{ 
					ml::Debug::log("Reloaded {0} Shaders.", ML_Res.shaders.reload()); 
				}

				// Close
				if (ev->getKeyDown(ml::KeyCode::Escape))
				{
					if (SETTINGS.escapeIsExit) { this->close(); } 
				}

				// Show Editor
				if (ev->getKeyDown(ml::KeyCode::E) && (ev->mods & ML_MOD_CTRL))
				{ show_ml_editor = true; }

				// Show Console
				if (ev->getKeyDown(ml::KeyCode::T) && ((ev->mods & ML_MOD_CTRL) && (ev->mods & ML_MOD_ALT)))
				{ show_ml_console = true; }

				// Show Network Manager
				if (ev->getKeyDown(ml::KeyCode::N) && ((ev->mods & ML_MOD_CTRL) && (ev->mods & ML_MOD_ALT)))
				{ show_ml_network = true; }

				// Show Shader Builder
				if (ev->getKeyDown(ml::KeyCode::B) && ((ev->mods & ML_MOD_CTRL) && (ev->mods & ML_MOD_ALT)))
				{ show_ml_shader = true; }
			}
			break;
		}

	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Demo::loadInterpreter()
	{
		static bool checked = false;
		if (!checked)
		{
			checked = true;

			// Setup Parser
			ML_Interpreter.parser()
				.showToks(SETTINGS.scrShowToks)
				.showTree(SETTINGS.scrShowTree)
				.showItoP(SETTINGS.scrShowItoP);

			// Load Commands

			ML_Interpreter.addCmd({ "/", [](ml::Args & args)
			{
				return ml::Var().intValue(ml::Debug::system(args.pop_front().str().c_str()));
			} });

			ML_Interpreter.addCmd({ "exit", [](ml::Args & args)
			{
				return ml::Var().voidValue();
			} });

			ML_Interpreter.addCmd({ "help", [](ml::Args & args)
			{
				for (auto n : ML_Interpreter.cmdNames())
				{
					ml::cout << n << ml::endl;
				}
				return ml::Var().boolValue(true);
			} });

			ML_Interpreter.addCmd({ "cwd", [](ml::Args & args)
			{
				const ml::String str = ML_FileSystem.getWorkingDir();
				if (args.pop_front().front() == "-p")
				{
					ml::cout << str;
				}
				return ml::Var().stringValue(str);
			} });

			ML_Interpreter.addCmd({ "cd", [](ml::Args & args)
			{
				return ml::Var().boolValue(ML_FileSystem.setWorkingDir(args.pop_front().front()));
			} });

			ML_Interpreter.addCmd({ "cat", [](ml::Args & args)
			{
				ml::String buf;
				if (ML_FileSystem.getFileContents(args.pop_front().front(), buf))
				{
					ml::cout << buf << ml::endl;

					return ml::Var().boolValue(true);
				}
				return ml::Var().boolValue(false);
			} });

			ML_Interpreter.addCmd({ "read", [](ml::Args & args)
			{
				ml::String name = args.pop_front().front();

				if (ML_FileSystem.fileExists(name))
				{
					ml::String buf;
					if (ML_FileSystem.getFileContents(name, buf))
					{
						return ml::Var().stringValue(buf);
					}
				}
				else
				{
					ml::SStream ss;
					if (ML_FileSystem.getDirContents(name, ss))
					{
						return ml::Var().stringValue(ss.str());
					}
				}
				return ml::Var().boolValue(false);
			} });

			ML_Interpreter.addCmd({ "exists", [](ml::Args & args)
			{
				auto name = args.pop_front().front();
				if (name == "." || name == "..")
				{
					return ml::Var().boolValue(true);
				}
				return ml::Var().boolValue(ML_FileSystem.fileExists(name));
			} });

			ML_Interpreter.addCmd({ "exec", [](ml::Args & args)
			{
				return ML_Interpreter.execFile(args.pop_front().front());
			} });

			ML_Interpreter.addCmd({ "ls", [](ml::Args & args)
			{
				const ml::String name = args.pop_front().empty() ? "." : args.str();
				ml::SStream ss;
				if (ML_FileSystem.getDirContents(name, ss))
				{
					ml::cout << ss.str();
					return ml::Var().boolValue(true);
				}
				return ml::Var().boolValue(false);
			} });

			ML_Interpreter.addCmd({ "target", [](ml::Args & args)
			{
				if (!args.pop_front().empty())
				{
					const ml::String & opt = args.front();
					if (opt == "name")
					{
						return ml::Var().stringValue(SETTINGS.title);
					}
					else if (opt == "config")
					{
					#ifdef ML_DEBUG
						return ml::Var().stringValue("Debug");
					#else
						return ml::Var().stringValue("Release");
					#endif
					}
					else if (opt == "platform")
					{
					#if defined(ML_x64)
						return ml::Var().stringValue("x64");
					#elif defined(ML_x86)
						return ml::Var().stringValue("x86");
					#endif
					}
				}
				return ml::Var().boolValue(true);
			} });

			ML_Interpreter.addCmd({ "load", [](ml::Args & args)
			{
				return ml::Var().boolValue(false);
			} });
		}
		return checked;
	}

	bool Demo::loadResources()
	{
		ml::Manifest manifest;
		if (manifest.loadFromFile(SETTINGS.pathTo(SETTINGS.manifest)))
		{
			ml::cout << ml::endl << manifest << ml::endl;

			return ml::Debug::log("Loading Resources...")
				&& ML_Res.loadManifest(manifest)
				&& ML_Res.models.load("borg")->loadFromMemory(ml::Shapes::Cube::Vertices, ml::Shapes::Cube::Indices)
				&& ML_Res.models.load("sanic")->loadFromMemory(ml::Shapes::Quad::Vertices, ml::Shapes::Quad::Indices)
				&& ML_Res.models.load("sprite")->loadFromMemory(ml::Shapes::Quad::Vertices, ml::Shapes::Quad::Indices)
				&& ML_Res.models.load("framebuffer")->loadFromMemory(ml::Shapes::Quad::Vertices, ml::Shapes::Quad::Indices)
				&& ML_Res.textures.load("framebuffer")->create(this->size())
				&& loadBuffers();
		}
		return ml::Debug::logError("Failed loading manifest");
	}
	
	bool Demo::loadBuffers()
	{
		// Text
		m_vaoText.create(ml::GL::Triangles).bind();
		m_vboText.create(ml::GL::DynamicDraw).bind();
		m_vboText.bufferData(NULL, (ml::Glyph::VertexCount * ml::Vertex::Size));
		ml::BufferLayout::Default.bind();
		m_vboText.unbind();
		m_vaoText.unbind();

		// Sprites
		m_sprVao.create(ml::GL::Triangles).bind();
		m_sprVbo.create(ml::GL::DynamicDraw).bind();
		m_sprVbo.bufferData(NULL, (ml::Glyph::VertexCount * ml::Vertex::Size));
		ml::BufferLayout::Default.bind();
		m_sprVbo.unbind();
		m_sprVao.unbind();

		// FBO
		m_fbo.create().bind();
		m_fbo.setTexture(ml::GL::ColorAttachment0, ML_Res.textures.get("framebuffer"));
		// RBO
		m_rbo.create(this->width(), this->height());
		m_rbo.bind();
		m_rbo.bufferStorage(ml::GL::Depth24_Stencil8);
		m_rbo.setFramebuffer(ml::GL::DepthStencilAttachment);
		m_rbo.unbind();
		// Check Framebuffer Status
		if (!ml::OpenGL::checkFramebufferStatus(ml::GL::Framebuffer))
		{
			return ml::Debug::logError("Framebuffer is not complete");
		}
		m_fbo.unbind();

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
			else if(SETTINGS.isClient)
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

		// Setup Interpreter
		if (loadInterpreter())
		{
			// Run Script
			if (!SETTINGS.scrFile.empty())
			{
				int32_t err = ML_Interpreter.execFile(
					SETTINGS.pathTo(SETTINGS.scrPath + SETTINGS.scrFile)
				)
				? ML_SUCCESS
				: ML_FAILURE;

				ml::Debug::setError(err);
			}
			else
			{
				ml::Debug::setError(ml::Debug::logWarning("No Script"));
			}
		}
		else
		{
			ml::Debug::setError(ML_FAILURE);
		}
	}
	
	void Demo::onLoad(const LoadEvent & ev)
	{
		ml::OpenGL::errorPause(SETTINGS.glErrorPause);

		const ml::Window::Style wStyle = ml::Window::Style::DefaultNoResize;
		const ml::VideoMode vMode(SETTINGS.windowSize(), SETTINGS.bitsPerPixel);
		const ml::Context cSettings(
			SETTINGS.majorVersion,
			SETTINGS.minorVersion,
			SETTINGS.profile,
			SETTINGS.depthBits,
			SETTINGS.stencilBits,
			SETTINGS.multisample,
			SETTINGS.srgbCapable
		);

		if (this->create(SETTINGS.title, vMode, wStyle, cSettings) && this->setup())
		{
			this->setInputMode(ml::Cursor::Normal);
			this->setPosition((ml::VideoMode::desktop().size - this->size()) / 2);
			this->setViewport(ml::vec2i::Zero, this->getFramebufferSize());

			if (ml::Debug::log("Initializing ImGui..."))
			{
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGui::StyleColorsDark();
				ImGui::GetStyle().FrameBorderSize = 1;
				if (!ImGui_ML_Init("#version 410", this, true))
				{
					ml::Debug::setError(ml::Debug::logError("Failed Loading ImGui"));
					return;
				}
			}

			if (ml::Debug::log("Initializing OpenAL..."))
			{
				if (!ml::OpenAL::init())
				{
					ml::Debug::setError(ml::Debug::logError("Failed Loading OpenAL"));
					return;
				}
			}

			if (ml::Debug::log("Initializing Resources..."))
			{
				if (!loadResources())
				{
					ml::Debug::setError(ml::Debug::logError("Failed Loading Resources"));
					return;
				}
			}

			if (ml::Debug::log("Initializing Network..."))
			{
				if (!loadNetwork())
				{
					ml::Debug::setError(ml::Debug::logError("Failed Loading Network"));
					return;
				}
			}
		}
		else
		{
			ml::Debug::setError(ml::Debug::logError("Failed Loading Window"));
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

		// Orthographic
		m_ortho = ml::Transform::Orthographic(
			{ ml::vec2f::Zero, (ml::vec2f)this->size() },
			{ SETTINGS.orthoNear, SETTINGS.orthoFar }
		);

		// Perspective
		m_persp = ml::Transform::Perspective(
			SETTINGS.fieldOfView,
			this->aspect(),
			{ SETTINGS.perspNear, SETTINGS.perspFar }
		);

		// Camera
		m_camera.LookAt(m_camPos, m_camPos + ml::vec3f::Back, ml::vec3f::Up);

		// Setup Model Transforms
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

		// Static Text
		m_text["static"]
			.setFont(ML_Res.fonts.get("minecraft"))
			.setFontSize(72)
			.setScale(ml::vec2f::One)
			.setPosition({ 32, 128 })
			.setColor(ml::Color::White)
			.setString("there is no need\nto be upset");

		// Threads
		m_thread = new ml::Thread([&]()
		{
			ml::Debug::log("Entering Dummy Thread");
			do
			{
				ml::cout << "*";
				ML_Time.sleep(1000);
			}
			while (this->isOpen());
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
		// Update Title
		this->setTitle(ml::String::Format(
			"{0} | {1} | {2} ({3} fps) | {4}",
			SETTINGS.title,
			ML_Time.elapsed(),
			ev.elapsed.delta(),
			ML_Time.calculateFPS(ev.elapsed.delta()),
			this->getCursorPos()
		));

		// Update Network
		if (SETTINGS.isServer)
		{
			ML_Server.poll();
		}
		else if(SETTINGS.isClient)
		{
			ML_Client.poll();
		}

		// Update Models
		{
			ML_Res.models.get("borg")->transform()
				.rotate(+ev.elapsed.delta(), ml::vec3f::One);

			ML_Res.models.get("cube")->transform()
				.rotate(-ev.elapsed.delta(), ml::vec3f::One);

			ML_Res.models.get("earth")->transform()
				.rotate((m_animate ? ev.elapsed.delta() : 0.f), ml::vec3f::Up);

			ML_Res.models.get("moon")->transform()
				.rotate(-ev.elapsed.delta(), ml::vec3f::Up);

			ML_Res.models.get("sanic")->transform()
				.rotate(-ev.elapsed.delta(), ml::vec3f::Forward);

			ML_Res.models.get("light")->transform()
				.setPosition(m_lightPos)
				.rotate(-ev.elapsed.delta(), ml::vec3f::Forward);
		}

		// Update Camera
		{
			const ml::vec3f pos = ML_Res.models.get("earth")->transform().getPosition();
			const ml::vec3f dir = (pos - m_camPos).normalized();

			ml::vec3f look = m_camPos + (pos - m_camPos).normalized();
			m_camera.lookAt(m_camPos, look, ml::vec3f::Up);

			float speed = (m_camAnimate ? m_camSpd * ev.elapsed.delta() : 0.0f);
			ml::vec3f fwd = (look - m_camPos);
			ml::vec3f right = (fwd.cross(ml::vec3f::Up) * ml::vec3f(1, 0, 1)).normalized();
			m_camPos += right * speed;
		}

	}
	
	void Demo::onDraw(const DrawEvent & ev)
	{
		// Shared Uniforms
		/* * * * * * * * * * * * * * * * * * * * */
		static ml::UniformSet persp_uniforms = {
			ml::Uniform("u_proj",	ml::Uniform::Mat4,	&m_persp.matrix()),
			ml::Uniform("u_view",	ml::Uniform::Mat4,	&m_camera.matrix()),
		};

		static ml::UniformSet light_uniforms = {
			ml::Uniform("u_viewPos",		ml::Uniform::Vec3,	&m_camPos),
			ml::Uniform("u_light.position",	ml::Uniform::Vec3,	&m_lightPos),
			ml::Uniform("u_light.color",	ml::Uniform::Vec4,	&m_lightCol),
			ml::Uniform("u_mat.ambient",	ml::Uniform::Float, &m_ambient),
			ml::Uniform("u_mat.specular",	ml::Uniform::Float, &m_specular),
			ml::Uniform("u_mat.shininess",	ml::Uniform::Int,	&m_shininess),
		};

		// Draw Scene
		/* * * * * * * * * * * * * * * * * * * * */
		m_fbo.bind();
		{
			// Clear
			this->clear(m_clearColor);
			
			ml::OpenGL::enable(ml::GL::CullFace);
			ml::OpenGL::enable(ml::GL::DepthTest);

			// Light
			if (const ml::Model * model = ML_Res.models.get("light"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_color",	ml::Uniform::Vec4,	&m_lightCol),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("solid"))
				{
					shader->setUniforms(persp_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Borg
			if (const ml::Model * model = ML_Res.models.get("borg"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_color",	ml::Uniform::Vec4,	&ml::Color::White),
					ml::Uniform("u_texture",ml::Uniform::Tex,	ML_Res.textures.get("borg")),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("basic"))
				{
					shader->setUniforms(persp_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Earth
			if (const ml::Model * model = ML_Res.models.get("earth"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",		ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_tex_dm",		ml::Uniform::Tex,	ML_Res.textures.get("earth_dm")),
					ml::Uniform("u_tex_sm",		ml::Uniform::Tex,	ML_Res.textures.get("earth_sm")),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("lighting"))
				{
					shader->setUniforms(persp_uniforms);
					shader->setUniforms(light_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Moon
			if (const ml::Model * model = ML_Res.models.get("moon"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",		ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_tex_dm",		ml::Uniform::Tex,	ML_Res.textures.get("moon_dm")),
					ml::Uniform("u_tex_sm",		ml::Uniform::Tex,	ML_Res.textures.get("moon_nm")),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("lighting"))
				{
					shader->setUniforms(persp_uniforms);
					shader->setUniforms(light_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Cube
			if (const ml::Model * model = ML_Res.models.get("cube"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_color",	ml::Uniform::Vec4,	&ml::Color::White),
					ml::Uniform("u_texture",ml::Uniform::Tex,	ML_Res.textures.get("stone_dm")),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("normal3D"))
				{
					shader->setUniforms(persp_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			ml::OpenGL::disable(ml::GL::CullFace);

			// Plane
			if (const ml::Model * model = ML_Res.models.get("ground"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_color",	ml::Uniform::Vec4,	&ml::Color::White),
					ml::Uniform("u_texture",ml::Uniform::Tex,	ML_Res.textures.get("stone_dm")),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("normal3D"))
				{
					shader->setUniforms(persp_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Sanic
			if(const ml::Model * model = ML_Res.models.get("sanic"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_color",	ml::Uniform::Vec4,	&ml::Color::White),
					ml::Uniform("u_texture",ml::Uniform::Tex,	ML_Res.textures.get("sanic")),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("basic"))
				{
					shader->setUniforms(persp_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			ml::OpenGL::disable(ml::GL::DepthTest);

			// Geometry
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_color",		ml::Uniform::Vec4,	&m_lineColor),
					ml::Uniform("u_lineMode",	ml::Uniform::Int,	&m_lineMode),
					ml::Uniform("u_lineDelta",	ml::Uniform::Float, &m_lineDelta),
					ml::Uniform("u_lineSize",	ml::Uniform::Float, &m_lineSize),
					ml::Uniform("u_lineSamples",ml::Uniform::Int,	&m_lineSamples),
				};
				if (const ml::Shader * shader = ML_Res.shaders.get("geometry"))
				{
					shader->setUniforms(uniforms);
					shader->bind();
				}
				ml::OpenGL::drawArrays(ml::GL::Points, 0, 4);
			}

			// Text
			if (true)
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_proj",	ml::Uniform::Mat4, &m_ortho.matrix()),
					ml::Uniform("u_color",	ml::Uniform::Vec4),
					ml::Uniform("u_texture",ml::Uniform::Tex),
				};
				static ml::RenderBatch batch(
					&m_vaoText,
					&m_vboText,
					ML_Res.shaders.get("text"),
					&uniforms);

				static const uint32_t  fontSize = 24;
				static const ml::vec2f offset = { 0.0f, -(float)fontSize };
				static const ml::vec2f origin = { (float)fontSize, (float)this->height() - 64 };

				// Dynamic Text
				size_t i = 0;
				for (auto pair : ML_Res.fonts.getAll())
				{
					this->draw(m_text["dynamic"]
						.setFont(pair.second)
						.setFontSize(fontSize)
						.setScale(ml::vec2f::One)
						.setPosition(origin + (offset * (float)(i + 1)))
						.setColor(ml::Color::White)
						.setString(pair.second->to_str())// + " | " + this->title())
					, batch);
					i++;
				}

				// Static Text
				this->draw(m_text["static"], batch);
			}

			// SPRITE
			if (const ml::Texture * texture = ML_Res.textures.get("icon"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_color",	ml::Uniform::Vec4,	&ml::Color::White),
					ml::Uniform("u_texture",ml::Uniform::Tex,	texture),
					ml::Uniform("u_proj",	ml::Uniform::Mat4,	&m_ortho.matrix()),
				};
				static ml::RenderBatch batch(
					&m_sprVao, 
					&m_sprVbo, 
					ML_Res.shaders.get("sprites"),
					&uniforms);

				auto drawSprite = [&](const ml::vec2f & pos, const ml::vec2f & scale, const float rot, const ml::vec2f & origin = { 0.5f })
				{
					ml::vec2f size = (ml::vec2f)texture->size() * scale;
					ml::vec2f dest = (pos - size * origin);
					this->draw(ml::Shapes::genQuadFloats({ dest, size }), batch);
				};

				drawSprite(((ml::vec2f)(this->size()) * ml::vec2f(0.95f, 0.075f)), { 0.5f }, 0.0f);
			}
		}
		m_fbo.unbind();

		// Draw Framebuffer
		/* * * * * * * * * * * * * * * * * * * * */
		if(ml::Shader * shader = ML_Res.shaders.get("framebuffer"))
		{
			static ml::UniformSet uniforms = {
				ml::Uniform("u_texture",ml::Uniform::Tex, ML_Res.textures.get("framebuffer")),
				ml::Uniform("u_mode",	ml::Uniform::Int, &m_fboMode),
			};
			shader->setUniforms(uniforms);
			shader->bind();
			this->clear(ml::Color::White);
			this->draw(*ML_Res.models.get("framebuffer"));
		}

		// Draw GUI
		/* * * * * * * * * * * * * * * * * * * * */
		this->pollEvents();
		
		ImGui_ML_NewFrame();
		ImGui::NewFrame();
		{
			ML_EventSystem.fireEvent(GuiEvent(ev.elapsed));
		}
		ImGui::Render();
		ImGui_ML_RenderDrawData(ImGui::GetDrawData());

		/* * * * * * * * * * * * * * * * * * * * */

		this->swapBuffers();

		/* * * * * * * * * * * * * * * * * * * * */
	}

	void Demo::onGui(const GuiEvent & ev)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		// Main Menu Bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N", false, false)) {}
				if (ImGui::MenuItem("Open", "Ctrl+O", false, false)) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl+S", false, false)) {}
				if (ImGui::MenuItem("Save All", "Ctrl+Shift+S", false, false)) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Quit", "Alt+F4")) { this->close(); }
				ImGui::EndMenu();
			}
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
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem("Editor", "Ctrl+E", &show_ml_editor);
				ImGui::MenuItem("Console", "Ctrl+Alt+T", &show_ml_console);
				ImGui::MenuItem("Network", "Ctrl+Alt+N", &show_ml_network);
				ImGui::MenuItem("Shader Builder", "Ctrl+Alt+B", &show_ml_shader);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("ImGui Demo", NULL, &show_imgui_demo);
				ImGui::MenuItem("ImGui Metrics", NULL, &show_imgui_metrics);
				ImGui::MenuItem("Style Editor", NULL, &show_imgui_style);
				ImGui::MenuItem("About Dear ImGui", NULL, &show_imgui_about);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Windows
		if (show_imgui_demo)	{ ImGui::ShowDemoWindow(&show_imgui_demo); }
		if (show_imgui_metrics) { ImGui::ShowMetricsWindow(&show_imgui_metrics); }
		if (show_imgui_style)	{ ImGui::Begin("Style Editor", &show_imgui_style); ImGui::ShowStyleEditor(); ImGui::End(); }
		if (show_imgui_about)	{ ImGui::ShowAboutWindow(&show_imgui_about); }
		
		// Editor
		if (show_ml_editor)
		{
			if (!ImGui::Begin("Editor", &show_ml_editor, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::End();
				return;
			}
			else
			{
				ML_Editor.ShowFramerate();
				ImGui::Separator();

				if (ImGui::Button("Reload Shaders"))
				{
					ml::Debug::log("Reloaded {0} Shaders.", ML_Res.shaders.reload());
				}
				ImGui::Separator();

				ImGui::Text("Scene");
				ImGui::ColorEdit4("Clear Color", &m_clearColor[0]);
				ImGui::Separator();

				ImGui::Text("Framebuffer");
				static ml::CString fbo_modes[] = {
						"Normal",
						"Grayscale",
						"Blur",
						"Juicy",
						"Inverted",
				};
				ImGui::Combo("Shader##Framebuffer", &m_fboMode, fbo_modes, IM_ARRAYSIZE(fbo_modes));
				ImGui::Separator();

				ImGui::Text("Camera");
				ImGui::Checkbox("Move##Camera", &m_camAnimate);
				ML_Editor.InputVec3f("Position##Camera", m_camPos);
				ImGui::DragFloat("Speed##Camera", &m_camSpd, 0.1f, -5.f, 5.f);
				ImGui::Separator();

				ImGui::Text("Light");
				ML_Editor.InputVec3f("Position##Light", m_lightPos);
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
				ML_Editor.InputTransform("Matrix", temp);
				ImGui::Separator();

				ImGui::End();
			}
		}

		// Console
		if (show_ml_console)
		{
			static ml::InterpreterConsole console;
			console.Draw("Console", &show_ml_console);
		}

		// Network
		if (show_ml_network)
		{
			if (!ImGui::Begin("Network", &show_ml_network, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::End();
				return;
			}
			else
			{
				ML_Editor.ShowHelpMarker("Some help text");
				ML_Editor.ShowFramerate();
				ImGui::Separator();

				ImGui::Text("Network Manager Placeholder");
				ImGui::End();
			}
		}

		// Shader
		if (show_ml_shader)
		{
			if (!ImGui::Begin("Shader Builder", &show_ml_shader, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::End();
				return;
			}
			else
			{
				// Shader Source
				ImGui::BeginGroup();
				{
					ImGui::Text("Source:");

					static ml::String source(1024, '\0');

					ImGui::InputTextMultiline(
						"##Source",
						&source[0],
						source.capacity(),
						{ 512, ImGui::GetTextLineHeight() * 16 },
						0);

					if (ImGui::Button("Compile")) {}
				}
				ImGui::EndGroup();

				// Uniform Editor
				ImGui::PushItemWidth(256);
				{
					// List Uniforms
					ImGui::SameLine();
					ImGui::BeginGroup();
					{
						const size_t count = m_uniforms.size();

						ImGui::Text("Uniforms:");

						// List
						if (ImGui::ListBoxHeader("##Uniforms", count))
						{
							for (size_t i = 0; i < count; i++)
							{
								const ml::String name = (std::to_string(i) + " : " + m_uniforms[i].name);
								if (ImGui::Selectable(name.c_str(), (i == m_selected)))
								{
									m_selected = i;
								}
							}
							ImGui::ListBoxFooter();
						}

						// Buttons
						ImGui::BeginGroup();
						{
							if (ImGui::Button("Up"))
							{
								if (m_selected > 0)
								{
									std::swap(m_uniforms[m_selected], m_uniforms[m_selected - 1]);
									m_selected--;
								}
							}
							ImGui::SameLine();
							if (ImGui::Button("Dn"))
							{
								if (m_selected + 1 < count)
								{
									std::swap(m_uniforms[m_selected], m_uniforms[m_selected + 1]);
									m_selected++;
								}
							}
							ImGui::SameLine();
							if (ImGui::Button("New"))
							{
								m_uniforms.push_back(ml::Uniform("new_uniform"));
								m_selected = m_uniforms.size() - 1;
							}
							ImGui::SameLine();
							if (ImGui::Button("Ins"))
							{
								m_uniforms.insert(m_uniforms.begin() + m_selected, ml::Uniform("new_uniform"));
							}
							ImGui::SameLine();
							if (ImGui::Button("Dup"))
							{
								if (count > 0)
								{
									m_uniforms.push_back(ml::Uniform(m_uniforms[m_selected]));
									m_selected = m_uniforms.size() - 1;
								}
							}
							ImGui::SameLine();
							if (ImGui::Button("Del"))
							{
								if (count > 0)
								{
									m_uniforms.erase(m_uniforms.begin() + m_selected);
									m_selected = (m_selected > 0 ? m_selected - 1 : m_uniforms.size() - 1);
								}
							}
						}
						ImGui::EndGroup();
					}
					ImGui::EndGroup();

					// Edit Uniforms
					ImGui::SameLine();
					ImGui::BeginGroup();
					{
						ImGui::Text("Edit:");

						static ml::CString u_types[] = {
							"None",
							"Int",
							"Float",
							"Vec2",
							"Vec3",
							"Vec4",
							"Mat3",
							"Mat4",
							"Tex",
						};

						if (ml::Uniform * u = (m_uniforms.empty() ? NULL : &m_uniforms[m_selected]))
						{
							ImGui::PushID(u->name.c_str());
							ImGui::InputText("Name", &u->name[0], 32);
							ImGui::Combo("Type", &u->type, u_types, IM_ARRAYSIZE(u_types));
							switch (u->type)
							{
							case ml::Uniform::Int:
							{
								static int32_t temp;
								ImGui::DragInt("Value", &temp);
							}
							break;
							case ml::Uniform::Float:
							{
								static float temp;
								ImGui::DragFloat("Value", &temp, 0.1f);
							}
							break;
							case ml::Uniform::Vec2:
							{
								static ml::vec2f temp;
								ML_Editor.InputVec2f("Value", temp);
							}
							break;
							case ml::Uniform::Vec3:
							{
								static ml::vec3f temp;
								ML_Editor.InputVec3f("Value", temp);
							}
							break;
							case ml::Uniform::Vec4:
							{
								static ml::vec4f temp;
								ML_Editor.InputVec4f("Value", temp);
							}
							break;
							case ml::Uniform::Mat3:
							{
								static ml::mat3f temp;
								ML_Editor.InputMat3f("Value", temp);
							}
							break;
							case ml::Uniform::Mat4:
							{
								static ml::mat4f temp;
								ML_Editor.InputMat4f("Value", temp);
							}
							break;
							case ml::Uniform::Tex:
								break;
							}
							ImGui::PopID();
						}
						else
						{
							ImGui::Text("Nothing Selected");
						}
					}
					ImGui::EndGroup();
				}
				ImGui::PopItemWidth();

				ImGui::End();
			}
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
}