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
		: m_error(ML_SUCCESS)
	{
		ML_EventSystem.addListener(DemoEvent::EV_Enter, this);
		ML_EventSystem.addListener(DemoEvent::EV_Load,	this);
		ML_EventSystem.addListener(DemoEvent::EV_Start, this);
		ML_EventSystem.addListener(DemoEvent::EV_Update,this);
		ML_EventSystem.addListener(DemoEvent::EV_Draw,	this);
		ML_EventSystem.addListener(DemoEvent::EV_Gui,	this);
		ML_EventSystem.addListener(DemoEvent::EV_Exit,	this);
	}

	Demo::~Demo()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Demo::onEvent(const ml::Event * value)
	{
		ml::RenderWindow::onEvent(value);

		switch (value->eventID())
		{
		case DemoEvent::EV_Enter:	return onEnter(*value->Cast<EnterEvent>());
		case DemoEvent::EV_Load:	return onLoad(*value->Cast<LoadEvent>());
		case DemoEvent::EV_Start:	return onStart(*value->Cast<StartEvent>());
		case DemoEvent::EV_Update:	return onUpdate(*value->Cast<UpdateEvent>());
		case DemoEvent::EV_Draw:	return onDraw(*value->Cast<DrawEvent>());
		case DemoEvent::EV_Gui:		return onGui(*value->Cast<GuiEvent>());
		case DemoEvent::EV_Exit:	return onExit(*value->Cast<ExitEvent>());

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
				switch (ev->button)
				{
				case ml::KeyCode::Escape:
					if (ev->action == ML_PRESS)
					{
						if (SETTINGS.escapeIsExit) { this->close(); }
					}
					break;
				case ml::KeyCode::R:
					if (ev->action == ML_PRESS)
					{
					}
					break;
				case ml::KeyCode::E:
					if (ev->action == ML_PRESS && (ev->mods & ML_MOD_CTRL))
					{
						show_ml_editor = true;
					}
					break;
				case ml::KeyCode::H:
					if (ev->action == ML_PRESS && (ev->mods & ML_MOD_CTRL) && (ev->mods & ML_MOD_ALT))
					{
						show_imgui_demo = true;
					}
					break;
				case ml::KeyCode::T:
					if (ev->action == ML_PRESS && (ev->mods & ML_MOD_CTRL) && (ev->mods & ML_MOD_ALT))
					{
						show_ml_console = true;
					}
					break;
				case ml::KeyCode::N:
					if (ev->action == ML_PRESS && (ev->mods & ML_MOD_CTRL) && (ev->mods & ML_MOD_ALT))
					{
						show_ml_network = true;
					}
					break;
				case ml::KeyCode::B:
					if (ev->action == ML_PRESS && (ev->mods & ML_MOD_CTRL) && (ev->mods & ML_MOD_ALT))
					{
						show_ml_shader = true;
					}
					break;
				}
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

			ML_Interpreter.addCmd({ "system", [](ml::Args & args)
			{
				return ml::Var().intValue(system(args.pop_front().str().c_str()));
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
		}
		return checked;
	}

	bool Demo::loadResources()
	{
		ml::Manifest manifest;
		if (manifest.loadFromFile(SETTINGS.pathTo(SETTINGS.manifest)))
		{
			ml::cout << ml::endl << manifest << ml::endl;

			return ML_Resources.loadManifest(manifest)
				&& ML_Resources.models.get("borg")->loadFromMemory(ml::Shapes::Cube::Vertices, ml::Shapes::Cube::Indices)
				&& ML_Resources.models.get("sanic")->loadFromMemory(ml::Shapes::Quad::Vertices, ml::Shapes::Quad::Indices)
				&& ML_Resources.textures.get("framebuffer")->create(this->size())
				&& loadBuffers()
				;
		}
		return ml::Debug::logError("Failed loading manifest");
	}
	
	bool Demo::loadBuffers()
	{
		// Load Buffers
		if (ml::Debug::log("Loading Buffers..."))
		{
			// Text
			m_vaoText
				.create(ml::GL::Triangles)
				.bind();
			m_vboText
				.create(ml::GL::DynamicDraw)
				.bind()
				.bufferData(NULL, (ml::Glyph::VertexCount * ml::Vertex::Size));
			ml::BufferLayout::Default.bind();
			m_vboText.unbind();
			m_vaoText.unbind();


			// FBO
			m_frameBuffer
				.create()
				.bind();
			// RBO
			m_renderBuffer
				.create(this->width(), this->height())
				.bind()
				.bufferStorage(ml::GL::Depth24_Stencil8)
				.bufferFramebuffer(ml::GL::DepthStencilAttachment)
				.unbind();
			// Texture
			ml::OpenGL::framebufferTexture2D(
				ml::GL::Framebuffer,
				ml::GL::ColorAttachment0,
				ml::GL::Texture2D,
				*ML_Resources.textures.get("framebuffer"),
				0);
			if (!ml::OpenGL::checkFramebufferStatus(ml::GL::Framebuffer))
			{
				return ml::Debug::logError("Framebuffer is not complete");
			}
			m_frameBuffer.unbind();
			
		}
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
				m_error = ML_Interpreter.execFile(
					SETTINGS.pathTo(SETTINGS.scrPath + SETTINGS.scrFile)
				)
				? ML_SUCCESS
				: ML_FAILURE;
			}
			else
			{
				m_error = ml::Debug::logWarning("No Script");
			}
		}
		else
		{
			m_error = ML_FAILURE;
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

			if (ml::Debug::log("Dear ImGui..."))
			{
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGui::StyleColorsDark();
				ImGui::GetStyle().FrameBorderSize = 1;
				if (!ImGui_ML_Init("#version 410", this, true))
				{
					m_error = ML_FAILURE;
					return;
				}
			}

			m_error = ml::Debug::log("Loading...")
				&& ml::OpenAL::init()
				&& loadResources()
				&& loadNetwork()
				? ML_SUCCESS
				: ML_FAILURE;
		}
		else
		{
			m_error = ml::Debug::logError("Failed Creating Window");
		}
	}
	
	void Demo::onStart(const StartEvent & ev)
	{
		ml::Debug::log("Starting...");

		// Set Window Icon
		if (ml::Image * icon = ML_Resources.images.get("icon"))
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
		ML_Resources.models.get("borg")->transform()
			.translate({ +5.0f, 0.0f, 0.0f });
		
		ML_Resources.models.get("sanic")->transform()
			.translate({ -5.0f, 0.0f, 0.0f });

		ML_Resources.models.get("earth")->transform()
			.translate({ 0.0f, 0.0f, 0.0f });

		ML_Resources.models.get("cube")->transform()
			.translate({ 0.0f, 0.0f, -5.0f })
			.scale(0.5f);

		ML_Resources.models.get("moon")->transform()
			.translate({ 0.0f, 0.0f, 5.0f })
			.scale(0.5f);

		ML_Resources.models.get("ground")->transform()
			.translate({ 0.0f, -2.5f, 0.0f })
			.scale({ 12.5, 0.25f, 12.5 });

		// Static Text
		m_text["static"]
			.setFont(ML_Resources.fonts.get("minecraft"))
			.setFontSize(72)
			.setScale(ml::vec2f::One)
			.setPosition({ 32, 128 })
			.setColor(ml::Color::White)
			.setString("there is no need\nto be upset");

		// Threads
		m_thread = new ml::Thread([&]()
		{
			using namespace ml;
			Debug::log("Entering Dummy Thread");
			do
			{
				cout << "*";
				ML_Time.sleep(1_s);
			}
			while (this->isOpen());
			cout << endl;
			Debug::log("Exiting Dummy Thread");
		});
		if (SETTINGS.enableThreads) { m_thread->launch(); }
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
			ML_Resources.models.get("borg")->transform()
				.rotate(+ev.elapsed.delta(), ml::vec3f::One);

			ML_Resources.models.get("cube")->transform()
				.rotate(-ev.elapsed.delta(), ml::vec3f::One);

			ML_Resources.models.get("earth")->transform()
				.rotate((m_animate ? ev.elapsed.delta() : 0.f), ml::vec3f::Up);

			ML_Resources.models.get("moon")->transform()
				.rotate(ev.elapsed.delta(), ml::vec3f::Up);

			ML_Resources.models.get("sanic")->transform()
				.rotate(-ev.elapsed.delta(), ml::vec3f::Forward);

			ML_Resources.models.get("light")->transform()
				.setPosition(m_lightPos)
				.rotate(-ev.elapsed.delta(), ml::vec3f::Forward);
		}

		// Update Camera
		{
			const ml::vec3f pos = ML_Resources.models.get("earth")->transform().getPosition();
			const ml::vec3f dir = (pos - m_camPos).normalized();

			m_camLook = m_camPos + (pos - m_camPos).normalized();
			m_camera.lookAt(m_camPos, m_camLook, ml::vec3f::Up);

			float speed = (m_camAnimate ? m_camSpd * ev.elapsed.delta() : 0.0f);
			ml::vec3f fwd = (m_camLook - m_camPos);
			ml::vec3f right = (fwd.cross(ml::vec3f::Up) * ml::vec3f(1, 0, 1)).normalized();
			m_camPos += right * speed;
		}
	}
	
	void Demo::onDraw(const DrawEvent & ev)
	{
		static ml::UniformSet camera_uniforms = {
			ml::Uniform("u_proj",	ml::Uniform::Mat4,	&m_persp.matrix()),
			ml::Uniform("u_view",	ml::Uniform::Mat4,	&m_camera.matrix()),
		};

		// Draw Scene
		/* * * * * * * * * * * * * * * * * * * * */
		m_frameBuffer.bind();
		{
			// Clear
			this->clear(m_clearColor);
			
			// 3D
			ml::OpenGL::enable(ml::GL::CullFace);
			ml::OpenGL::enable(ml::GL::DepthTest);

			// Light
			if (const ml::Model * model = ML_Resources.models.get("light"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_color",	ml::Uniform::Vec4,	&m_lightCol),
				};
				if (const ml::Shader * shader = ML_Resources.shaders.get("solid"))
				{
					shader->setUniforms(camera_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Borg
			if (const ml::Model * model = ML_Resources.models.get("borg"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_color",	ml::Uniform::Vec4,	&ml::Color::White),
					ml::Uniform("u_texture",ml::Uniform::Tex2D, ML_Resources.textures.get("borg")),
				};
				if (const ml::Shader * shader = ML_Resources.shaders.get("basic3D"))
				{
					shader->setUniforms(camera_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Earth
			if (const ml::Model * model = ML_Resources.models.get("earth"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",		ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_viewPos",	ml::Uniform::Vec3,	&m_camPos),
					ml::Uniform("u_lightPos",	ml::Uniform::Vec3,	&m_lightPos),
					ml::Uniform("u_lightCol",	ml::Uniform::Vec4,	&m_lightCol),
					ml::Uniform("u_ambient",	ml::Uniform::Float, &m_ambient),
					ml::Uniform("u_specular",	ml::Uniform::Float, &m_specular),
					ml::Uniform("u_shininess",	ml::Uniform::Int,	&m_shininess),
					ml::Uniform("u_tex_dm",		ml::Uniform::Tex2D,	ML_Resources.textures.get("earth_dm")),
					ml::Uniform("u_tex_sm",		ml::Uniform::Tex2D,	ML_Resources.textures.get("earth_sm")),
				};
				if (const ml::Shader * shader = ML_Resources.shaders.get("lighting"))
				{
					shader->setUniforms(camera_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Moon
			if (const ml::Model * model = ML_Resources.models.get("moon"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",		ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_viewPos",	ml::Uniform::Vec3,	&m_camPos),
					ml::Uniform("u_lightPos",	ml::Uniform::Vec3,	&m_lightPos),
					ml::Uniform("u_lightCol",	ml::Uniform::Vec4,	&m_lightCol),
					ml::Uniform("u_ambient",	ml::Uniform::Float, &m_ambient),
					ml::Uniform("u_specular",	ml::Uniform::Float, &m_specular),
					ml::Uniform("u_shininess",	ml::Uniform::Int,	&m_shininess),
					ml::Uniform("u_tex_dm",		ml::Uniform::Tex2D,	ML_Resources.textures.get("moon_dm")),
					ml::Uniform("u_tex_sm",		ml::Uniform::Tex2D,	ML_Resources.textures.get("moon_nm")),
				};
				if (const ml::Shader * shader = ML_Resources.shaders.get("lighting"))
				{
					shader->setUniforms(camera_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Cube
			if (const ml::Model * model = ML_Resources.models.get("cube"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_color",	ml::Uniform::Vec4,	&ml::Color::White),
					ml::Uniform("u_texture",ml::Uniform::Tex2D, ML_Resources.textures.get("stone_dm")),
				};
				if (const ml::Shader * shader = ML_Resources.shaders.get("normal3D"))
				{
					shader->setUniforms(camera_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// 2D
			ml::OpenGL::disable(ml::GL::CullFace);

			// Plane
			if (const ml::Model * model = ML_Resources.models.get("ground"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_color",	ml::Uniform::Vec4,	&ml::Color::White),
					ml::Uniform("u_texture",ml::Uniform::Tex2D,	ML_Resources.textures.get("stone_dm")),
				};
				if (const ml::Shader * shader = ML_Resources.shaders.get("normal3D"))
				{
					shader->setUniforms(camera_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// Sanic
			if(const ml::Model * model = ML_Resources.models.get("sanic"))
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_model",	ml::Uniform::Mat4,	&model->transform().matrix()),
					ml::Uniform("u_color",	ml::Uniform::Vec4,	&ml::Color::White),
					ml::Uniform("u_texture",ml::Uniform::Tex2D,	ML_Resources.textures.get("sanic")),
				};
				if (const ml::Shader * shader = ML_Resources.shaders.get("basic3D"))
				{
					shader->setUniforms(camera_uniforms);
					shader->setUniforms(uniforms);
					shader->bind();
				}
				this->draw(*model);
			}

			// GUI
			ml::OpenGL::disable(ml::GL::DepthTest);

			// Text
			if (true)
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_proj",	ml::Uniform::Mat4, &m_ortho.matrix()),
					ml::Uniform("u_color",	ml::Uniform::Vec4),
					ml::Uniform("u_texture",ml::Uniform::Tex2D),
				};
				static ml::RenderBatch batch(
					&m_vaoText,
					&m_vboText,
					ML_Resources.shaders.get("text"),
					&uniforms);

				static const uint32_t  fontSize = 24;
				static const ml::vec2f offset = { 0.0f, -(float)fontSize };
				static const ml::vec2f origin = { (float)fontSize, (float)this->height() - 64 };

				// Dynamic Text
				size_t i = 0;
				for (auto pair : ML_Resources.fonts.getAll())
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

			// Geometry
			{
				static ml::UniformSet uniforms = {
					ml::Uniform("u_color",		ml::Uniform::Vec4,	&m_lineColor),
					ml::Uniform("u_lineMode",	ml::Uniform::Int,	&m_lineMode),
					ml::Uniform("u_lineDelta",	ml::Uniform::Float, &m_lineDelta),
					ml::Uniform("u_lineSamples",ml::Uniform::Int,	&m_lineSamples),
				};
				if (const ml::Shader * shader = ML_Resources.shaders.get("geometry"))
				{
					shader->setUniforms(uniforms);
					shader->bind();
				}
				ml::OpenGL::drawArrays(ml::GL::Points, 0, 4);
			}

		}
		m_frameBuffer.unbind();

		// Draw Framebuffer
		/* * * * * * * * * * * * * * * * * * * * */
		if(ml::Shader * shader = ML_Resources.shaders.get("framebuffer"))
		{
			static ml::UniformSet uniforms = {
				ml::Uniform("u_texture",ml::Uniform::Tex2D, ML_Resources.textures.get("framebuffer")),
				ml::Uniform("u_mode",	ml::Uniform::Int,	&m_fboMode),
			};
			shader->setUniforms(uniforms);
			shader->bind();
			this->clear(ml::Color::White);
			this->draw(*ML_Resources.models.get("sanic"));
		}

		// Draw GUI
		/* * * * * * * * * * * * * * * * * * * * */
		this->pollEvents();
		
		ImGui_ML_NewFrame();
		ImGui::NewFrame();
		{
			ML_EventSystem.fireEvent(GuiEvent(ev.elapsed, ev.input));
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
				ImGui::MenuItem("ImGui Demo", "Ctrl+Alt+H", &show_imgui_demo);
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
				ML_Editor.ShowHelpMarker("Some help text");
				ML_Editor.ShowFramerate();
				ImGui::Separator();

				if (ImGui::BeginTabBar("EditorTabBar", ImGuiTabBarFlags_None))
				{
					if (ImGui::BeginTabItem("Scene"))
					{
						ImGui::ColorEdit4("Clear Color", &m_clearColor[0]);
						ImGui::Separator();

						static ml::CString fbo_modes[] = {
								"Normal",
								"Grayscale",
								"Blur",
								"Juicy",
								"Inverted",
						};
						ImGui::Combo("Framebuffer", &m_fboMode, fbo_modes, IM_ARRAYSIZE(fbo_modes));
						ImGui::Separator();

						ImGui::Checkbox("Camera Move", &m_camAnimate);
						ML_Editor.InputVec3f("Camera Pos", m_camPos);
						ML_Editor.InputVec3f("Camera Look", m_camLook);
						ImGui::DragFloat("Camera Speed", &m_camSpd, 0.01f, 0.f, 1.f);
						ImGui::Separator();

						ML_Editor.InputVec3f("Light Position", m_lightPos);
						ImGui::ColorEdit4("Light Color", &m_lightCol[0]);
						ImGui::DragFloat("Light Ambient", &m_ambient, 0.01f, 0.f, 1.f);
						ImGui::DragFloat("Light Specular", &m_specular, 0.01f, 0.1f, 10.f);
						ImGui::DragInt("Light Shininess", &m_shininess, 1.f, 1, 256);
						ImGui::Separator();

						ImGui::SliderInt("Line Mode", &m_lineMode, -1, 3);
						ImGui::ColorEdit4("Line Color", &m_lineColor[0]);
						ImGui::SliderFloat("Line Delta", &m_lineDelta, 0.f, 1.f);
						ImGui::SliderInt("Line Samples", &m_lineSamples, 1, 128);
						ImGui::Separator();

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Transform"))
					{
						ImGui::Checkbox("Animate", &m_animate);

						ml::Transform & temp = ML_Resources.models.get("earth")->transform();

						ML_Editor.InputTransform("Matrix", temp);

						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}

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
							"Int",
							"Float",
							"Vec2",
							"Vec3",
							"Vec4",
							"Mat3",
							"Mat4",
							"Tex2D",
						};

						if (ml::Uniform * u = (m_uniforms.empty() ? NULL : &m_uniforms[m_selected]))
						{
							ImGui::InputText("Name", &u->name[0], 32);
							ImGui::Combo("Type", &u->type, u_types, IM_ARRAYSIZE(u_types));
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

		ML_Resources.clean();

		ImGui_ML_Shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}