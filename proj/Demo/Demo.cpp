#include "Demo.hpp"
#include "InterpreterConsole.hpp"
#include <MemeCore/EventSystem.hpp>
#include <MemeWindow/WindowEvents.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <../thirdparty/include/dirent.h>
#endif

#include <imgui/imgui.h>
#include <imgui/imgui_ml.hpp>

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * */

	Demo::Demo()
		: m_status(ml::Debug::Success)
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
		case DemoEvent::EV_Enter:	onEnter(*value->Cast<EnterEvent>());	break;
		case DemoEvent::EV_Load:	onLoad(*value->Cast<LoadEvent>());		break;
		case DemoEvent::EV_Start:	onStart(*value->Cast<StartEvent>());	break;
		case DemoEvent::EV_Update:	onUpdate(*value->Cast<UpdateEvent>());	break;
		case DemoEvent::EV_Draw:	onDraw(*value->Cast<DrawEvent>());		break;
		case DemoEvent::EV_Gui:		onGui(*value->Cast<GuiEvent>());		break;
		case DemoEvent::EV_Exit:	onExit(*value->Cast<ExitEvent>());		break;

		case ml::WindowEvent::EV_WindowSize:
			if (auto ev = value->Cast<ml::WindowSizeEvent>())
			{
				// Orthographic
				proj[P_ortho] = ml::Transform::Orthographic(
					0.0f, (float)this->width(),
					0.0f, (float)this->height(),
					SETTINGS.orthoNear,
					SETTINGS.orthoFar);

				// Perspective
				proj[P_persp] = ml::Transform::Perspective(
					SETTINGS.fieldOfView,
					this->aspect(),
					SETTINGS.perspNear,
					SETTINGS.perspFar);
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
						this->close();
					}
					break;
				case ml::KeyCode::E:
					if (ev->action == ML_PRESS && (ev->mods & ML_MOD_CTRL))
					{
						show_ml_editor = true;
					}
					break;
				case ml::KeyCode::T:
					if (ev->action == ML_PRESS && (ev->mods & ML_MOD_CTRL) && (ev->mods & ML_MOD_ALT))
					{
						show_ml_console = true;
					}
					break;
				}
			}
			break;
		}

	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Demo::setupInterpreter()
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
					ml::String::Stream ss;
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
				const ml::String dirName = args.pop_front().empty() ? "./" : args.str();
				if (DIR * dir = opendir(dirName.c_str()))
				{
					while (dirent * e = readdir(dir))
					{
						switch (e->d_type)
						{
						case DT_REG:
							ml::cout << (ml::FG::Green | ml::BG::Black) << e->d_name << "";
							break;
						case DT_DIR:
							ml::cout << (ml::FG::Blue | ml::BG::Green) << e->d_name << "/";
							break;
						case DT_LNK:
							ml::cout << (ml::FG::Green | ml::BG::Black) << e->d_name << "@";
							break;
						default:
							ml::cout << (ml::FG::Green | ml::BG::Black) << e->d_name << "*";
							break;
						}
						ml::cout << ml::FMT() << ml::endl;
					}
					closedir(dir);
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

	bool Demo::loadFonts()
	{
		// Load Fonts
		return ml::Debug::log("Loading Fonts...") && ML_Resources.loadFonts({
			{ "clacon",		SETTINGS.pathTo("fonts/clacon.ttf") },
			{ "consolas",	SETTINGS.pathTo("fonts/consolas.ttf") },
			{ "lconsole",	SETTINGS.pathTo("fonts/lucida_console.ttf") },
			{ "minecraft",	SETTINGS.pathTo("fonts/minecraft.ttf") },
		});
	}

	bool Demo::loadImages()
	{
		// Load Images
		return ml::Debug::log("Loading Images...") && ML_Resources.loadImages({
			{ "icon",		SETTINGS.pathTo("/images/dean.png")} 
		});
	}

	bool Demo::loadTextures()
	{
		// Load Textures
		return ml::Debug::log("Loading Textures...") && ML_Resources.loadTextures({
			{ "dean",		SETTINGS.pathTo("/images/dean.png") },
			{ "sanic",		SETTINGS.pathTo("/images/sanic.png") },
			{ "earth",		SETTINGS.pathTo("/images/earth.png") },
			//{ "bg_clouds",	SETTINGS.pathTo("/textures/bg/bg_clouds.png") },
			//{ "sky_clouds", SETTINGS.pathTo("/textures/bg/sky_clouds.png") },
			//{ "sky_water",	SETTINGS.pathTo("/textures/bg/sky_water.png") },
			//{ "earth_cm",	SETTINGS.pathTo("/textures/earth/earth_cm_2k.png") },
			//{ "earth_dm",	SETTINGS.pathTo("/textures/earth/earth_dm_2k.png") },
			//{ "earth_hm",	SETTINGS.pathTo("/textures/earth/earth_hm_2k.png") },
			//{ "earth_lm",	SETTINGS.pathTo("/textures/earth/earth_lm_2k.png") },
			//{ "earth_nm",	SETTINGS.pathTo("/textures/earth/earth_nm_2k.png") },
			//{ "earth_sm",	SETTINGS.pathTo("/textures/earth/earth_sm_2k.png") },
			//{ "mars_dm",	SETTINGS.pathTo("/textures/mars/mars_dm_2k.png") },
			//{ "mars_nm",	SETTINGS.pathTo("/textures/mars/mars_nm_2k.png") },
			//{ "moon_dm",	SETTINGS.pathTo("/textures/moon/moon_dm_2k.png") },
			//{ "moon_nm",	SETTINGS.pathTo("/textures/moon/moon_nm_2k.png") },
			{ "stone_dm",	SETTINGS.pathTo("/textures/stone/stone_dm.png") },
			{ "stone_hm",	SETTINGS.pathTo("/textures/stone/stone_hm.png") },
			{ "stone_nm",	SETTINGS.pathTo("/textures/stone/stone_nm.png") },
		});
	}

	bool Demo::loadShaders()
	{
		// Load Shaders
		return ml::Debug::log("Loading Shaders...") && ML_Resources.loadShaders({
			{ "basic3D",	SETTINGS.pathTo("/shaders/basic3D.shader") },
			{ "sprites",	SETTINGS.pathTo("/shaders/sprites.shader") },
			{ "text",		SETTINGS.pathTo("/shaders/text.shader") },
			{ "geometry",	SETTINGS.pathTo("/shaders/geometry.shader") },
			{ "framebuffer",SETTINGS.pathTo("/shaders/framebuffer.shader") },
		});
	}

	bool Demo::loadUniforms()
	{
		return ml::Debug::log("Loading Uniforms...")
			&& (uniforms[U_Model]	= "u_model")
			&& (uniforms[U_View]	= "u_view")
			&& (uniforms[U_Proj]	= "u_proj")
			&& (uniforms[U_Color]	= "u_color")
			&& (uniforms[U_Tex]		= "u_texture")
			&& (uniforms[U_Curve]	= "u_curveMode")
			;
	}

	bool Demo::loadMeshes()
	{
		// Load Meshes
		return ml::Debug::log("Loading Meshes...") && ML_Resources.loadMeshes({
			{ "example",	SETTINGS.pathTo("/meshes/example.mesh") },
			{ "sphere8x6",	SETTINGS.pathTo("/meshes/sphere8x6.mesh") },
			{ "sphere32x24",SETTINGS.pathTo("/meshes/sphere32x24.mesh") },
		});
	}

	bool Demo::loadModels()
	{
		return ml::Debug::log("Loading Models...")
			;
	}

	bool Demo::loadBuffers()
	{
		// Load Buffers
		if (ml::Debug::log("Loading Buffers..."))
		{
			static const ml::BufferLayout layout({
				{ 0, 3, ml::GL::Float, false, ml::Vertex::Size, 0, sizeof(float) },
				{ 1, 4, ml::GL::Float, false, ml::Vertex::Size, 3, sizeof(float) },
				{ 2, 2, ml::GL::Float, false, ml::Vertex::Size, 7, sizeof(float) },
			});

			// Cube
			vao[VAO_cube]
				.create(ml::GL::Triangles)
				.bind();
			vbo[VBO_cube]
				.create(ml::GL::StaticDraw)
				.bind()
				.bufferData(ml::Shapes::Cube::Vertices.contiguous());
			ibo[IBO_cube]
				.create(ml::GL::StaticDraw, ml::GL::UnsignedInt)
				.bind()
				.bufferData(ml::Shapes::Cube::Indices);
			layout.bind();
			ibo[IBO_cube].unbind();
			vbo[VBO_cube].unbind();
			vao[VAO_cube].unbind();


			// Quad
			vao[VAO_quad]
				.create(ml::GL::Triangles)
				.bind();
			vbo[VBO_quad]
				.create(ml::GL::StaticDraw)
				.bind()
				.bufferData(ml::Shapes::Quad::Vertices.contiguous());
			ibo[IBO_quad]
				.create(ml::GL::StaticDraw, ml::GL::UnsignedInt)
				.bind()
				.bufferData(ml::Shapes::Quad::Indices);
			layout.bind();
			ibo[IBO_quad].unbind();
			vbo[VBO_quad].unbind();
			vao[VAO_quad].unbind();


			// Example
			vao[VAO_example]
				.create(ml::GL::Triangles)
				.bind();
			vbo[VBO_example]
				.create(ml::GL::StaticDraw)
				.bind()
				.bufferData(ML_Resources.getMesh("example")->contiguous());
			layout.bind();
			vbo[VBO_example].unbind();
			vao[VAO_example].unbind();


			// Sphere 8x6
			vao[VAO_sphere8x6]
				.create(ml::GL::Triangles)
				.bind();
			vbo[VBO_sphere8x6]
				.create(ml::GL::StaticDraw)
				.bind()
				.bufferData(ML_Resources.getMesh("sphere8x6")->contiguous());
			layout.bind();
			vbo[VBO_sphere8x6].unbind();
			vao[VAO_sphere8x6].unbind();


			// Sphere 32x24
			vao[VAO_sphere32x24]
				.create(ml::GL::Triangles)
				.bind();
			vbo[VAO_sphere32x24]
				.create(ml::GL::StaticDraw)
				.bind()
				.bufferData(ML_Resources.getMesh("sphere32x24")->contiguous());
			layout.bind();
			vbo[VBO_sphere32x24].unbind();
			vao[VAO_sphere32x24].unbind();

			
			// Text
			vao[VAO_text]
				.create(ml::GL::Triangles)
				.bind();
			vbo[VBO_text]
				.create(ml::GL::DynamicDraw)
				.bind()
				.bufferData(NULL, (ml::Glyph::VertexCount * ml::Vertex::Size));
			layout.bind();
			vbo[VBO_text].unbind();
			vao[VAO_text].unbind();


			// FBO
			fbo[FBO_scene]
				.create()
				.bind();
			// RBO
			rbo[RBO_scene]
				.create(this->width(), this->height())
				.bind()
				.bufferStorage(ml::GL::Depth24_Stencil8)
				.bufferFramebuffer(ml::GL::DepthStencilAttachment)
				.unbind();
			// Texture
			if (m_framebuffer.create(
				rbo[RBO_scene].width(),
				rbo[RBO_scene].height()
			))
			{
				ml::OpenGL::framebufferTexture2D(
					ml::GL::Framebuffer, 
					ml::GL::ColorAttachment0,
					ml::GL::Texture2D,
					m_framebuffer,
					0);
			}
			if (!ml::OpenGL::checkFramebufferStatus(ml::GL::Framebuffer))
			{
				return ml::Debug::logError("Framebuffer is not complete");
			}
			fbo[FBO_scene].unbind();
			
		}
		return true;
	}

	bool Demo::loadAudio()
	{
		if (ml::Debug::log("Loading Audio..."))
		{
			if (ml::OpenAL::init())
			{
				return true;
			}
			else
			{
				return ml::Debug::logError("Failed Loading Audio");
			}
		}
		return ml::Debug::log("Loading Sounds...")
			;
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
						ml::Debug::log("Server Started: {0}", ML_Server.getMyAddress());
						return true;
					}
				}
			}
			else if(SETTINGS.isClient)
			{
				// Client Setup
				if (ML_Client.setup())
				{
					if (ML_Client.connect({ ML_LOCALHOST, ML_PORT }, ""))
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
		// Start Master Timer
		ML_Time.start();

		// Setup Interpreter
		if (setupInterpreter())
		{
			// Run Script
			if (!SETTINGS.scrFile.empty())
			{
				m_status = ML_Interpreter.execFile(
					SETTINGS.pathTo(SETTINGS.scrPath + SETTINGS.scrFile)
				)
				? ml::Debug::Success
				: ml::Debug::Error;
			}
			else
			{
				m_status = ml::Debug::logWarning("No Script");
			}
		}
		else
		{
			m_status = ml::Debug::Error;
		}
	}
	
	void Demo::onLoad(const LoadEvent & ev)
	{
		ml::OpenGL::errorPause(true);

		if (ml::Debug::log("Creating Window...") && create(
			SETTINGS.title,
			ml::VideoMode({ SETTINGS.width, SETTINGS.height }, SETTINGS.bitsPerPixel),
			ml::Window::Style::Default,
			ml::Context(
				SETTINGS.majorVersion,
				SETTINGS.minorVersion,
				SETTINGS.profile,
				SETTINGS.depthBits,
				SETTINGS.stencilBits,
				SETTINGS.multisample,
				SETTINGS.srgbCapable)
		)
		&& setup() // Initialize OpenGL
		)
		{
			this->setInputMode(ml::Cursor::Normal);
			this->setPosition((ml::VideoMode::desktop().size - this->size()) / 2);
			this->setViewport(ml::vec2i::Zero, this->size());

			if (ml::Debug::log("Dear ImGui..."))
			{
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGui::StyleColorsDark();
				ImGui::GetStyle().FrameBorderSize = 1;
				if (!ImGui_ML_Init("#version 410", this, true))
				{
					m_status = ml::Debug::Error;
					return;
				}
			}

			m_status = (ml::Debug::log("Loading Resources..."))
				&& loadFonts()
				&& loadImages()
				&& loadTextures()
				&& loadShaders()
				&& loadUniforms()
				&& loadMeshes()
				&& loadModels()
				&& loadBuffers()
				&& loadAudio()
				&& loadNetwork()
				? ml::Debug::Success
				: ml::Debug::Error;
		}
		else
		{
			m_status = ml::Debug::logError("Failed Creating Window");
		}
	}
	
	void Demo::onStart(const StartEvent & ev)
	{
		if (ml::Debug::log("Starting..."))
		{
			// Set Window Icon
			if (ml::Image * icon = ML_Resources.getImage("icon"))
			{
				this->setIcons({ (*icon).flipVertically() });
			}

			// Orthographic
			proj[P_ortho] = ml::Transform::Orthographic(
				0.0f, (float)this->width(),
				0.0f, (float)this->height(),
				SETTINGS.orthoNear,
				SETTINGS.orthoFar);

			// Perspective
			proj[P_persp] = ml::Transform::Perspective(
				SETTINGS.fieldOfView,
				this->aspect(),
				SETTINGS.perspNear,
				SETTINGS.perspFar);

			// Camera
			ml::vec3f cameraPos = { 0.0f, 0.0f, 3.0f };
			view[V_camera].lookAt(
				cameraPos,
				cameraPos + ml::vec3f::Back,
				ml::vec3f::Up);

			// Cube
			transform[T_cube]
				.translate({ +5.0f, 0.0f, 0.0f })
				.rotate(0.0f, ml::vec3f::Up)
				.scale(ml::vec3f::One);

			// Quad
			transform[T_quad]
				.translate({ -5.0f, 0.0f, 0.0f })
				.rotate(0.0f, ml::vec3f::Up)
				.scale(ml::vec3f::One);

			// Sphere 32x24
			transform[T_sphere32x24]
				.translate({ 0.0f, 0.0f, 0.0f })
				.rotate(0.0f, ml::vec3f::Up)
				.scale(ml::vec3f::One);

			// Static Text
			text[TXT_static]
				.setFont(ML_Resources.getFont("minecraft"))
				.setFontSize(72)
				.setScale(ml::vec2f::One)
				.setPosition({ 32, 128 })
				.setColor(ml::Color::White)
				.setText("there is no need\nto be upset");
		}
	}
	
	void Demo::onUpdate(const UpdateEvent & ev)
	{
		// Update Title
		this->setTitle(ml::String::Format(
			"{0} | {1} | {2} ({3} fps) | {4}",
			SETTINGS.title,
			ML_Time.elapsed(),
			ev.elapsed.delta(),
			ml::Time::calculateFPS(ev.elapsed.delta()),
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
	}
	
	void Demo::onDraw(const DrawEvent & ev)
	{
		// Draw Scene
		/* * * * * * * * * * * * * * * * * * * * */
		fbo[FBO_scene].bind();
		{
			// Clear
			this->clear(m_clearColor);
			
			// 3D
			ml::OpenGL::enable(ml::GL::CullFace);
			ml::OpenGL::enable(ml::GL::DepthTest);

			// Cube
			if (ml::Shader * s = ML_Resources.getShader("basic3D"))
			{
				s->setUniform(uniforms[U_Color],	ml::Color::White);
				s->setUniform(uniforms[U_Tex],		(*ML_Resources.getTexture("stone_dm")));
				s->setUniform(uniforms[U_Proj],		proj[P_persp]);
				s->setUniform(uniforms[U_View],		view[V_camera]);
				s->setUniform(uniforms[U_Model],	transform[T_cube]
					.translate(ml::vec3f::Zero)
					.rotate(+ev.elapsed.delta(), ml::vec3f::One)
					.scale(ml::vec3f::One));
				s->bind();

				this->draw(
					vao[VAO_cube],
					vbo[VBO_cube],
					ibo[IBO_cube]);
			}
			
			// Sphere32x24
			if (ml::Shader * s = ML_Resources.getShader("basic3D"))
			{
				s->setUniform(uniforms[U_Color],	ml::Color::White);
				s->setUniform(uniforms[U_Tex],		(*ML_Resources.getTexture("earth")));
				s->setUniform(uniforms[U_Proj],		proj[P_persp]);
				s->setUniform(uniforms[U_View],		view[V_camera]);
				s->setUniform(uniforms[U_Model],	transform[T_sphere32x24]
					.translate(ml::vec3f::Zero)
					.rotate(+ev.elapsed.delta(), ml::vec3f::Up)
					.scale(ml::vec3f::One)
				);
				s->bind();

				this->draw(
					vao[VAO_sphere32x24], 
					vbo[VBO_sphere32x24]);
			}

			// 2D
			ml::OpenGL::disable(ml::GL::CullFace);
			ml::OpenGL::disable(ml::GL::DepthTest);

			// Quad
			if (ml::Shader * s = ML_Resources.getShader("sprites"))
			{
				s->setUniform(uniforms[U_Color],	ml::Color::White);
				s->setUniform(uniforms[U_Tex],		(*ML_Resources.getTexture("sanic")));
				s->setUniform(uniforms[U_Proj],		proj[P_persp]);
				s->setUniform(uniforms[U_View],		view[V_camera]);
				s->setUniform(uniforms[U_Model],	transform[T_quad]
					.translate(ml::vec3f::Zero)
					.rotate(-ev.elapsed.delta(), ml::vec3f::Forward)
					.scale(ml::vec3f::One));
				s->bind();

				this->draw(
					vao[VAO_quad], 
					vbo[VBO_quad], 
					ibo[IBO_quad]);
			}

			// Text
			if (true)
			{
				static ml::RenderBatch batch(
					&vao[VAO_text],
					&vbo[VBO_text],
					&proj[P_ortho],
					ML_Resources.getShader("text"));

				static const uint32_t  fontSize = 24;
				static const ml::vec2f offset = { 0.0f, -(float)fontSize };
				static const ml::vec2f origin = { (float)fontSize, (float)this->height() - 64 };

				// Dynamic Text
				size_t i = 0;
				for (auto pair : ML_Resources.getAllFonts())
				{
					this->draw(text[TXT_dynamic]
						.setFont(pair.second)
						.setFontSize(fontSize)
						.setScale(ml::vec2f::One)
						.setPosition(origin + (offset * (float)(i + 1)))
						.setColor(ml::Color::White)
						.setText(pair.second->to_str())// + " | " + this->title())
					, batch);
					i++;
				}

				// Static Text
				this->draw(text[TXT_static], batch);
			}

			// Geometry
			if (ml::Shader * s = ML_Resources.getShader("geometry"))
			{
				s->setUniform(uniforms[U_Color], m_lineColor);
				s->setUniform(uniforms[U_Curve], m_lineMode);
				s->setUniform("u_delta", m_lineDelta);
				s->setUniform("u_samples", m_lineSamples);
				s->bind();
				ml::OpenGL::drawArrays(ml::GL::Points, 0, 4);
			}

		}
		fbo[FBO_scene].unbind();


		// Draw Framebuffer
		/* * * * * * * * * * * * * * * * * * * * */
		if(ml::Shader * s = ML_Resources.getShader("framebuffer"))
		{
			this->clear(ml::Color::White);

			s->setUniform(uniforms[U_Tex], m_framebuffer);
			s->setUniform("u_mode", m_fboMode);
			s->bind();
			
			this->draw(
				vao[VAO_quad],
				vbo[VBO_quad],
				ibo[IBO_quad]);
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
		this->makeContextCurrent();
		this->setViewport(ml::vec2f::Zero, this->getFramebufferSize());
		ImGui_ML_RenderDrawData(ImGui::GetDrawData());
		this->makeContextCurrent();

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
		
		// Console
		if (show_ml_console)
		{
			static ml::InterpreterConsole console;
			console.Draw("MemeScript Console", &show_ml_console);
		}

		// Editor
		if (show_ml_editor)
		{
			//ImGui::SetNextWindowPos(ImVec2(780, 200), ImGuiCond_Once);
			//ImGui::SetNextWindowSize(ImVec2(450, 250), ImGuiCond_Once);

			if (!ImGui::Begin("Editor", &show_ml_editor, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::End();
				return;
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Separator();

			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
			{
				if (ImGui::BeginTabItem("Scene"))
				{
					ImGui::ColorEdit4("Clear Color", &m_clearColor[0]);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Framebuffer"))
				{
					ImGui::SliderInt("FBO Mode", &m_fboMode, 0, 4);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Geometry"))
				{
					ImGui::SliderInt("Line Mode", &m_lineMode, 0, 3);
					ImGui::ColorEdit4("Line Color", &m_lineColor[0]);
					ImGui::SliderFloat("Line Delta", &m_lineDelta, 0.f, 1.f);
					ImGui::SliderInt("Line Samples", &m_lineSamples, 1, 128);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Source"))
				{
					enum { Size = 128 };
					static char buf[Size] = "Hello, World!";
					ImGui::InputTextMultiline("Label", buf, Size, { 0, 0 }, 0);
					if (ImGui::Button("Compile")) {}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Model"))
				{
					ml::mat4f temp = transform[T_sphere32x24];
					ML_Editor.InputMat4f("Sphere", temp);
					transform[T_sphere32x24] = temp;
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

			ImGui::End();
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}
	
	void Demo::onExit(const ExitEvent & ev)
	{
		ML_Resources.clearMeshes();
		ML_Resources.clearShaders();
		ML_Resources.clearTextures();
		ML_Resources.clearImages();
		ML_Resources.clearFonts();

		ImGui_ML_Shutdown();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}